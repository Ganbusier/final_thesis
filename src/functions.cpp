#include <easy3d/core/point_cloud.h>
#include <easy3d/fileio/point_cloud_io.h>
#include <easy3d/renderer/drawable_lines.h>
#include <energyMinimization/energyMinimization.h>
#include <graph/graph.h>
#include <ransac/ransac3d.h>
#include <regionGrowing/cgalDefines_rg.h>
#include <regionGrowing/regionGrowing.h>

#include "functions.h"

std::vector<easy3d::Drawable*> drawables;

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

  std::string preservedFilename = saveFilePath + "gco_preserved.ply";
  std::string removedFilename = saveFilePath + "gco_removed.ply";
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
    cylinderDrawable->set_uniform_coloring(
        easy3d::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    cylinderDrawable->update_vertex_buffer({start, end});
    cylinderDrawable->update_element_buffer({0, 1});
    viewer->add_drawable(cylinderDrawable);
    drawables.push_back(cylinderDrawable);
  }

  std::string cylinderInfosFilename = saveFilePath + "rg_cylinderInfos.csv";
  std::string unassignedPointsFilename = saveFilePath + "rg_unassignedPoints.ply";
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

  ransac::Ransac3d ransac3d(pointCloud, params);
  ransac3d.detect();

  std::vector<ransac::CylinderInfo> cylinderInfos =
      ransac3d.getCylinderInfos();
  
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
    cylinderDrawable->set_uniform_coloring(
        easy3d::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    cylinderDrawable->update_vertex_buffer({start, end});
    cylinderDrawable->update_element_buffer({0, 1});
    viewer->add_drawable(cylinderDrawable);
    drawables.push_back(cylinderDrawable);
  }

  std::string cylinderInfosFilename = saveFilePath + "ransac3d_cylinderInfos.csv";
  std::string leftoverPointsFilename = saveFilePath + "ransac3d_leftoverPoints.ply";
  ransac3d.saveCylinderInfos(cylinderInfosFilename);
  ransac3d.saveLeftoverPoints(leftoverPointsFilename);

  return true;
}
