#ifndef ANALYSIS_H
#define ANALYSIS_H

#include <easy3d/core/graph.h>

#include <map>
#include <set>
#include <utility>
#include <vector>

namespace myAnalysis {

struct AnalysisResults {
  float minDistance = -1.0f;
  float maxDistance = -1.0f;
  float minAngle = -1.0f;
  float maxAngle = -1.0f;
  float meanDistance = -1.0f;
  float meanAngle = -1.0f;
  float medianDistance = -1.0f;
  float medianAngle = -1.0f;
  float stdDistance = -1.0f;
  float stdAngle = -1.0f;
  float RMSEofMeanDistances = -1.0f;
  float RMSEofMeanAngles = -1.0f;
};

class Analysis {
 public:
  Analysis(easy3d::Graph* estimatedGraph, easy3d::Graph* groundTruthGraph)
      : m_estimatedGraph(estimatedGraph),
        m_groundTruthGraph(groundTruthGraph) {}
  ~Analysis() {}

  void analyze();
  std::vector<float> getMeanDistances() const { return m_meanDistances; }
  std::vector<float> getMeanAngles() const { return m_meanAngles; }
  std::vector<easy3d::Graph::Edge> getUnmatchedEstimatedEdges() const {
    return m_unmatchedEstimatedEdges;
  }
  std::vector<easy3d::Graph::Edge> getUnmatchedGroundTruthEdges() const {
    return m_unmatchedGroundTruthEdges;
  }
  AnalysisResults getAnalysisResults() const { return m_analysisResults; }

 private:
  easy3d::Graph* m_estimatedGraph;
  easy3d::Graph* m_groundTruthGraph;
  std::vector<float> m_meanDistances;
  std::vector<float> m_meanAngles;
  AnalysisResults m_analysisResults;

  // Storage for grouped matches: ground truth edge -> vector of matched
  // estimated edges
  std::map<easy3d::Graph::Edge, std::vector<easy3d::Graph::Edge>>
      m_matchedEdges;

  // Storage for unmatched edges
  std::vector<easy3d::Graph::Edge> m_unmatchedEstimatedEdges;
  std::vector<easy3d::Graph::Edge> m_unmatchedGroundTruthEdges;

  void distanceAnalysis();
  void angleAnalysis();
  void matchEdges();
  void calculateStatistics();
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
