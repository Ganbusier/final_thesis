#ifndef ENERGY_MINIMIZATION_H
#define ENERGY_MINIMIZATION_H

#include <easy3d/core/graph.h>
#include <easy3d/core/point_cloud.h>
#include <easy3d/kdtree/kdtree_search_eth.h>
#include <gco/GCoptimization.h>
#include <queue>
#include <unordered_set>
#include <vector>

// define a has function for std::pair<int, int> to use in std::unordered_set
namespace std {
template <> struct hash<std::pair<int, int>> {
  size_t operator()(const std::pair<int, int> &p) const {
    return hash<long long>()(((long long)p.first) ^
                             (((long long)p.second) << 32));
  }
};
} // namespace std

namespace energyMinimization {

struct NeighborPair {
  int node1_idx = -1;
  int node2_idx = -1;
  float weight = 0.0f;
};

struct DataTermParams {
  float extendFactor = 2.0f;
  float inlierSearchRadius = 1.0f;
  float minAngleInDegrees = 10.0f;
};

class EnergyMinimization {
private:
  int m_numNodes = 0;
  int m_numNeighborPairs = 0;
  int m_scaleFactor =
      100; // scale both term from [0, 1] to [0, scaleFactor], float to int
  int m_numLabels =
      2; // for wireframe extraction, 2 labels: removed (0) or preserved (1)
  float m_lambda = 0.1; // weight for the smoothness term
  int m_V[4] = {0, 1, 1,
                0}; // V[label1 + num_label*label2] --> V(0,0), V(1,0), V(0,1),
                    // V(1,1) must satisfy: V(0, 0) + V(1, 1) <= V(0,1) + V(1,0)
  DataTermParams m_dataTermParams;

  GCoptimizationGeneralGraph *m_gc;
  easy3d::Graph *m_graph;
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
                     easy3d::PointCloud *pointCloud);
  ~EnergyMinimization();
  void setDataTerm();
  void setSmoothnessTerm();
  void optimize();
  void getResults(std::vector<int> &preserved, std::vector<int> &removed);
};

} // namespace energyMinimization

#endif // ENERGY_MINIMIZATION_H