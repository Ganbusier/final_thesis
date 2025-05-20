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