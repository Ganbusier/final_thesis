#include <easy3d/core/point_cloud.h>
#include <easy3d/fileio/point_cloud_io.h>
#include <easy3d/renderer/drawable_lines.h>

#include "functions.h"


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
  energyMinimization.saveResults(saveFilePath);

  return true;
}

bool run_RegionGrowing(easy3d::Viewer* viewer, easy3d::Model* model,
                       const std::string& saveFilePath) {
  if (!viewer || !model) return false;
  auto pointCloud = dynamic_cast<easy3d::PointCloud*>(model);

  Point_set pointSet;
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

  std::vector<regionGrowing::Cylinder> cylinders;
  regionGrowing.getCylinders(cylinders);

  if (cylinders.empty()) {
    LOG(INFO) << "No cylinders found";
    return true;
  }

  for (size_t i = 0; i < cylinders.size(); ++i) {
    auto cylinder = cylinders[i];
    easy3d::vec3 start = easy3d::vec3(cylinder.start.x(), cylinder.start.y(), cylinder.start.z());
    easy3d::vec3 end = easy3d::vec3(cylinder.end.x(), cylinder.end.y(), cylinder.end.z());
    auto cylinderDrawable = new easy3d::LinesDrawable("cylinder" + std::to_string(i));
    cylinderDrawable->set_impostor_type(easy3d::LinesDrawable::CYLINDER);
    cylinderDrawable->set_line_width(cylinder.radius * 2.0f * 10.0f);
    cylinderDrawable->set_uniform_coloring(easy3d::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    cylinderDrawable->update_vertex_buffer({start, end});
    cylinderDrawable->update_element_buffer({0, 1});
    viewer->add_drawable(cylinderDrawable);
  }

  return true;
}

bool run_Ransac3d(easy3d::Viewer* viewer, easy3d::Model* model,
                  const std::string& saveFilePath) {
  if (!viewer || !model) return false;
  auto pointCloud = dynamic_cast<easy3d::PointCloud*>(model);

  ransac::CustomCylinderRANSAC ransac;
  ransac.setEpsilon(0.05f);
  ransac.setBitmapResolution(0.5f);
  ransac.setNormalThreshold(0.9f);
  ransac.setMinSupport(10);
  ransac.setProbability(0.01f);
  ransac.detect(pointCloud);
  // ransac.setRadiusRange(0.01f, 1.0f);

  std::vector<ransac::CustomCylinder> cylinders = ransac.getCylinders();
  std::vector<int> leftOverIndices = ransac.getLeftOverIndices();

  if (cylinders.empty()) {
    return true;
  }

  for (size_t i = 0; i < cylinders.size(); ++i) {
    ransac::CustomCylinder cylinder = cylinders[i];
    easy3d::vec3 start = cylinder.start;
    easy3d::vec3 end = cylinder.end;
    auto cylinderDrawable = new easy3d::LinesDrawable("cylinder" + std::to_string(i));
    cylinderDrawable->set_impostor_type(easy3d::LinesDrawable::CYLINDER);
    cylinderDrawable->set_line_width(cylinder.radius * 2.0f);
    cylinderDrawable->set_uniform_coloring(easy3d::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    cylinderDrawable->update_vertex_buffer({start, end});
    cylinderDrawable->update_element_buffer({0, 1});
    viewer->add_drawable(cylinderDrawable);
  }

  // log some statistics for checking
  float minRadius = std::numeric_limits<float>::infinity();
  float maxRadius = 0.0;
  float meanRadius = 0.0;
  for (size_t i = 0; i < cylinders.size(); ++i) {
    float radius = cylinders[i].radius;
    minRadius = std::min(minRadius, radius);
    maxRadius = std::max(maxRadius, radius);
    meanRadius += radius;
  }
  meanRadius /= cylinders.size();
  LOG(INFO) << "Min radius: " << minRadius;
  LOG(INFO) << "Max radius: " << maxRadius;
  LOG(INFO) << "Mean radius: " << meanRadius;
  
  return true;
}
