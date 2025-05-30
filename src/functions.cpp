#include <easy3d/core/point_cloud.h>
#include <easy3d/fileio/point_cloud_io.h>
#include <easy3d/renderer/drawable_lines.h>
#include <energyMinimization/energyMinimization.h>
#include <graph/graph.h>
#include <ransac/ransac3d.h>
#include <ransac/ransac3d2d.h>
#include <regionGrowing/cgalDefines_rg.h>
#include <regionGrowing/regionGrowing.h>

#include "functions.h"

std::vector<easy3d::Drawable*> drawables;

// Global line color variable
static easy3d::vec4 current_line_color(0.984f, 0.333f, 0.490f, 1.0f); // Default pink #FB557D

// Function to get current line color
easy3d::vec4 get_current_line_color() {
    return current_line_color;
}

// Function to set line color
void set_line_drawable_color(const easy3d::vec4& color) {
    current_line_color = color;
    // Update color of all existing line drawables
    for (auto& drawable : drawables) {
        auto lines_drawable = dynamic_cast<easy3d::LinesDrawable*>(drawable);
        if (lines_drawable) {
            lines_drawable->set_uniform_coloring(color);
        }
    }
}

bool run_EnergyMinimization(easy3d::Viewer* viewer, easy3d::Model* model,
                            const std::string& saveFilePath) {
  if (!viewer || !model) return false;
  auto pointCloud = dynamic_cast<easy3d::PointCloud*>(model);

  graph::Graph graph(pointCloud, 10, 2.0f);
  graph.buildMixGraph();
  auto mixGraph = graph.getMixGraph();
  graph.buildDualGraph(mixGraph);
  auto dualGraph = graph.getDualGraph();

  energyMinimization::EnergyMinimization energyMinimization(
      dualGraph->n_vertices(), dualGraph->n_edges(), mixGraph, pointCloud);
  energyMinimization.setDataTerm();
  energyMinimization.setSmoothnessTerm();
  energyMinimization.optimize();
  energyMinimization.getResults();

  std::string preservedFilename = saveFilePath + "/gco_preserved.ply";
  std::string removedFilename = saveFilePath + "/gco_removed.ply";
  energyMinimization.saveResults(preservedFilename, removedFilename);

  return true;
}

bool run_RegionGrowing(easy3d::Viewer* viewer, easy3d::Model* model,
                       const std::string& saveFilePath) {
  if (!viewer || !model) return false;
  auto pointCloud = dynamic_cast<easy3d::PointCloud*>(model);

  regionGrowing::Point_set pointSet;
  regionGrowing::makePointSet(pointCloud, pointSet, 16);
  regionGrowing::CylinderRegionGrowingParams params;
  params.k = 16;
  params.maxDistance = 0.1;
  params.maxAngle = 25;
  params.minRadius = 0.01;
  params.maxRadius = 1.0;
  params.minRegionSize = 4;

  regionGrowing::CylinderRegionGrowing regionGrowing(pointSet, params);
  regionGrowing.detect();

  std::vector<regionGrowing::Cylinder> cylinders = regionGrowing.getCylinders();

  if (cylinders.empty()) {
    LOG(INFO) << "No cylinders found";
    return true;
  }

  for (auto& drawable : drawables) {
    viewer->delete_drawable(drawable);
  }
  drawables.clear();

  for (size_t i = 0; i < cylinders.size(); ++i) {
    auto cylinder = cylinders[i];
    easy3d::vec3 start = easy3d::vec3(cylinder.start.x(), cylinder.start.y(),
                                      cylinder.start.z());
    easy3d::vec3 end =
        easy3d::vec3(cylinder.end.x(), cylinder.end.y(), cylinder.end.z());
    auto cylinderDrawable =
        new easy3d::LinesDrawable("cylinder" + std::to_string(i));
    cylinderDrawable->set_impostor_type(easy3d::LinesDrawable::CYLINDER);
    cylinderDrawable->set_line_width(cylinder.radius * 2.0f * 10.0f);
    cylinderDrawable->set_uniform_coloring(get_current_line_color());
    cylinderDrawable->update_vertex_buffer({start, end});
    cylinderDrawable->update_element_buffer({0, 1});
    viewer->add_drawable(cylinderDrawable);
    drawables.push_back(cylinderDrawable);
  }

  std::string cylinderInfosFilename = saveFilePath + "/rg_cylinderInfos.csv";
  std::string unassignedPointsFilename =
      saveFilePath + "/rg_unassignedPoints.ply";
  regionGrowing.saveCylinderInfos(cylinderInfosFilename);
  regionGrowing.saveUnassignedPoints(unassignedPointsFilename);

  return true;
}

bool run_Ransac3d(easy3d::Viewer* viewer, easy3d::Model* model,
                  const std::string& saveFilePath) {
  if (!viewer || !model) return false;
  auto pointCloud = dynamic_cast<easy3d::PointCloud*>(model);

  ransac::Params params;
  params.normalThreshold = 0.9;
  params.probability = 0.01;
  params.minPoints = 10;
  params.epsilon = 0.05;
  params.clusterEpsilon = 1.0;
  params.minRadius = 0.01;
  params.maxRadius = 1.0;

  ransac::Ransac3d ransac3d(pointCloud, params);
  ransac3d.detect();

  std::vector<ransac::CylinderInfo> cylinderInfos = ransac3d.getCylinderInfos();

  if (cylinderInfos.empty()) {
    LOG(INFO) << "No cylinders found";
    return true;
  }

  for (auto& drawable : drawables) {
    viewer->delete_drawable(drawable);
  }
  drawables.clear();

  for (size_t i = 0; i < cylinderInfos.size(); ++i) {
    auto cylinderInfo = cylinderInfos[i];
    auto start = cylinderInfo.start;
    auto end = cylinderInfo.end;
    auto radius = cylinderInfo.radius;
    auto cylinderDrawable =
        new easy3d::LinesDrawable("cylinder" + std::to_string(i));
    cylinderDrawable->set_impostor_type(easy3d::LinesDrawable::CYLINDER);
    cylinderDrawable->set_line_width(radius * 2.0f);
    cylinderDrawable->set_uniform_coloring(get_current_line_color());
    cylinderDrawable->update_vertex_buffer({start, end});
    cylinderDrawable->update_element_buffer({0, 1});
    viewer->add_drawable(cylinderDrawable);
    drawables.push_back(cylinderDrawable);
  }

  std::string cylinderInfosFilename =
      saveFilePath + "/ransac3d_cylinderInfos.csv";
  std::string leftoverPointsFilename =
      saveFilePath + "/ransac3d_leftoverPoints.ply";
  ransac3d.saveCylinderInfos(cylinderInfosFilename);
  ransac3d.saveLeftoverPoints(leftoverPointsFilename);

  return true;
}

bool run_Ransac3d2d(easy3d::Viewer* viewer, easy3d::Model* model,
                    const std::string& saveFilePath) {
  if (!viewer || !model) return false;
  auto pointCloud = dynamic_cast<easy3d::PointCloud*>(model);

  ransac::Params_ransac3d2d params;
  params.normalThreshold = 0.0;
  params.probability = 0.01;
  params.minPoints = 10;
  params.epsilon = 0.05;
  params.clusterEpsilon = 1.0;
  params.maxIterations = 200;
  params.minInliers = 4;
  params.tolerance = 0.1;
  params.splitDistanceThres = 2.0;

  ransac::Ransac3d2d ransac3d2d(pointCloud, params);
  ransac3d2d.detect();

  std::vector<std::vector<ransac::Line3d>> lines3d = ransac3d2d.getLines3d();
  if (lines3d.empty()) {
    LOG(INFO) << "No lines found";
    return true;
  }

  for (auto& drawable : drawables) {
    viewer->delete_drawable(drawable);
  }
  drawables.clear();

  for (size_t i = 0; i < lines3d.size(); ++i) {
    std::vector<ransac::Line3d> lines = lines3d[i];
    for (size_t j = 0; j < lines.size(); ++j) {
      ransac::Line3d line = lines[j];
      auto start = line.start;
      auto end = line.end;
      auto lineDrawable = new easy3d::LinesDrawable(
          "line_" + std::to_string(i) + "_" + std::to_string(j));
      lineDrawable->set_impostor_type(easy3d::LinesDrawable::CYLINDER);
      lineDrawable->set_line_width(2.0f);
      lineDrawable->set_uniform_coloring(get_current_line_color());
      lineDrawable->update_vertex_buffer({start, end});
      lineDrawable->update_element_buffer({0, 1});
      viewer->add_drawable(lineDrawable);
      drawables.push_back(lineDrawable);
    }
  }

  // Save results
  std::string linesFilename = saveFilePath + "/ransac3d2d_lines.ply";
  std::string leftoverPointsFilename =
      saveFilePath + "/ransac3d2d_leftoverPoints.ply";
  ransac3d2d.saveLines3d(linesFilename);
  ransac3d2d.saveLeftoverPoints(leftoverPointsFilename);

  return true;
}

void clear_algorithm_drawables() {
  drawables.clear();
}
