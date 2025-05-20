#include <easy3d/core/point_cloud.h>
#include <easy3d/fileio/point_cloud_io.h>

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

  for (const auto& cylinder : cylinders) {
    LOG(INFO) << "Cylinder: " << cylinder.start << " " << cylinder.end << " "
              << cylinder.radius << " " << cylinder.length;
  }

  return true;
}
