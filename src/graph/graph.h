#ifndef GRAPH_H
#define GRAPH_H

#include <easy3d/algo/delaunay_3d.h>
#include <easy3d/core/graph.h>
#include <easy3d/core/point_cloud.h>
#include <easy3d/fileio/graph_io.h>
#include <easy3d/kdtree/kdtree_search_eth.h>

namespace graph {
class Graph {
 private:
  easy3d::Graph *m_knnGraph;
  easy3d::Graph *m_dtGraph;
  easy3d::Graph *m_mixGraph;
  easy3d::Graph *m_dualGraph;
  easy3d::PointCloud *m_pointCloud;

  int m_k = 16;                  // number of neighbors for KNN graph
  float m_maxEdgeLength = 2.0f;  // maximum edge length used for filter
                                 // dt graph with building mixed graph

 public:
  Graph(easy3d::PointCloud *pointCloud, int k = 16, float maxEdgeLength = 2.0f);
  ~Graph();
  void buildKNNGraph();
  void buildDTGraph();
  void buildMixGraph();
  void buildDualGraph(const easy3d::Graph *graph);
  easy3d::Graph *getKNNGraph() const;
  easy3d::Graph *getDTGraph() const;
  easy3d::Graph *getMixGraph() const;
  easy3d::Graph *getDualGraph() const;
  easy3d::PointCloud *getPointCloud() const;
  void saveGraph(const std::string &filename, const easy3d::Graph *graph,
                 bool binary = false) const;
  void loadGraph(const std::string &filename, easy3d::Graph *graph);
};
}  // namespace graph

#endif  // GRAPH_H
