#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <easy3d/core/graph.h>

namespace myAnalysis {
class Analysis {
 public:
  Analysis(easy3d::Graph* estimatedGraph, easy3d::Graph* groundTruthGraph)
      : m_estimatedGraph(estimatedGraph),
        m_groundTruthGraph(groundTruthGraph) {}
  ~Analysis() {}

  void analyze();

 private:
  easy3d::Graph* m_estimatedGraph;
  easy3d::Graph* m_groundTruthGraph;
  std::vector<float> m_distances;
  std::vector<float> m_angles;
  std::vector<std::vector<easy3d::Graph::Edge>> m_matchedEdges;

  void distanceAnalysis();
  void angleAnalysis();
  void matchEdges();
  float distanceBetweenTwoEdges(easy3d::Graph::Edge estimatedEdge,
                                easy3d::Graph::Edge groundTruthEdge);
  float angleBetweenTwoEdges(easy3d::Graph::Edge estimatedEdge,
                            easy3d::Graph::Edge groundTruthEdge);
  
  // Helper functions for distance calculation
  float pointToLineDistance(const easy3d::vec3& point, 
                           const easy3d::vec3& lineStart, 
                           const easy3d::vec3& lineEnd);
  bool isPointOnSegment(const easy3d::vec3& point,
                       const easy3d::vec3& segmentStart,
                       const easy3d::vec3& segmentEnd);
};
}  // namespace myAnalysis
#endif  // ANALYSIS_H
