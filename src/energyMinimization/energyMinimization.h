#ifndef ENERGY_MINIMIZATION_H
#define ENERGY_MINIMIZATION_H

#include <easy3d/core/graph.h>
#include <easy3d/core/point_cloud.h>
#include <easy3d/fileio/graph_io.h>
#include <easy3d/kdtree/kdtree_search_eth.h>
#include <gco/GCoptimization.h>

#include <queue>
#include <unordered_set>
#include <vector>

// define a has function for std::pair<int, int> to use in std::unordered_set
namespace std {
template <>
struct hash<std::pair<int, int>> {
  size_t operator()(const std::pair<int, int> &p) const {
    return hash<long long>()(((long long)p.first) ^
                             (((long long)p.second) << 32));
  }
};
}  // namespace std

namespace energyMinimization {

struct NeighborPair {
  int node1_idx = -1;
  int node2_idx = -1;
  float weight = 0.0f;
};

struct EnergyMinimizationParams {
  float lambda = 0.1f;
  float extendFactor = 2.0f;
  float inlierSearchRadius = 1.0f;
  float minAngleInDegrees = 10.0f;
};

class EnergyMinimization {
 private:
  /*
  m_numNodes: number of nodes in the graph

  m_numNeighborPairs: number of neighbor pairs in the graph

  m_scaleFactor: scale factor for the smoothness term, scale both term
  from [0, 1] to [0, scaleFactor], float to int

  m_numLabels: number of labels in the graph, for wireframe extraction, 2
  labels: removed (0) or preserved (1)

  m_lambda: weight for the smoothness term

  m_V: V(0,0), V(1,0), V(0,1), V(1,1), must satisfy: V(0, 0) + V(1, 1) <= V(0,1)
  + V(1,0)
  */
  int m_numNodes = 0;
  int m_numNeighborPairs = 0;
  int m_scaleFactor = 100;
  int m_numLabels = 2;
  int m_V[4] = {0, 1, 1, 0};

  EnergyMinimizationParams m_params;

  GCoptimizationGeneralGraph *m_gc;
  easy3d::Graph *m_graph;
  easy3d::Graph *m_preservedGraph;
  easy3d::Graph *m_removedGraph;
  easy3d::PointCloud *m_pointCloud;
  std::vector<int> m_dataTerm;
  std::vector<float> m_inlierProbTerm;
  std::vector<float> m_edgeLengthTerm;
  std::vector<NeighborPair> m_neighborPairs;

 private:
  void computeDataTerm();
  void computeInlierProbTerm();
  void computeEdgeLengthTerm();
  void computeNeighborPairWeights();

 public:
  EnergyMinimization(int num_nodes, int num_neighborPairs, easy3d::Graph *graph,
                     easy3d::PointCloud *pointCloud,
                     const EnergyMinimizationParams &params);
  ~EnergyMinimization();
  void setDataTerm();
  void setSmoothnessTerm();
  void optimize();
  void getResults();
  void saveResults(const std::string &preservedFilename,
                   const std::string &removedFilename);

  easy3d::Graph *getPreservedGraph() const { return m_preservedGraph; }
  easy3d::Graph *getRemovedGraph() const { return m_removedGraph; }
};

}  // namespace energyMinimization

#endif  // ENERGY_MINIMIZATION_H