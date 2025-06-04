#include "analysis.h"

namespace myAnalysis {
void Analysis::analyze() {
  matchEdges();
  distanceAnalysis();
  angleAnalysis();
}

// TODO: Implement these three functions
void Analysis::matchEdges() {}
void Analysis::distanceAnalysis() {}
void Analysis::angleAnalysis() {}

float Analysis::angleBetweenTwoEdges(easy3d::Graph::Edge estimatedEdge,
                                     easy3d::Graph::Edge groundTruthEdge) {
  easy3d::Graph::Vertex sourceEstimated =
      m_estimatedGraph->source(estimatedEdge);
  easy3d::Graph::Vertex targetEstimated =
      m_estimatedGraph->target(estimatedEdge);
  easy3d::Graph::Vertex sourceGroundTruth =
      m_groundTruthGraph->source(groundTruthEdge);
  easy3d::Graph::Vertex targetGroundTruth =
      m_groundTruthGraph->target(groundTruthEdge);

  easy3d::vec3 sourceEstimatedPos = m_estimatedGraph->position(sourceEstimated);
  easy3d::vec3 targetEstimatedPos = m_estimatedGraph->position(targetEstimated);
  easy3d::vec3 sourceGroundTruthPos =
      m_groundTruthGraph->position(sourceGroundTruth);
  easy3d::vec3 targetGroundTruthPos =
      m_groundTruthGraph->position(targetGroundTruth);

  easy3d::vec3 dir1 = (targetEstimatedPos - sourceEstimatedPos).normalize();
  easy3d::vec3 dir2 = (targetGroundTruthPos - sourceGroundTruthPos).normalize();

  float cosine = easy3d::dot(dir1, dir2);
  float angle = std::acos(std::clamp(cosine, -1.0f, 1.0f)) * 180.0f / M_PI;
  return angle;
}

float Analysis::distanceBetweenTwoEdges(easy3d::Graph::Edge estimatedEdge,
                                        easy3d::Graph::Edge groundTruthEdge) {
  easy3d::Graph::Vertex sourceEstimated =
      m_estimatedGraph->source(estimatedEdge);
  easy3d::Graph::Vertex targetEstimated =
      m_estimatedGraph->target(estimatedEdge);
  easy3d::Graph::Vertex sourceGroundTruth =
      m_groundTruthGraph->source(groundTruthEdge);
  easy3d::Graph::Vertex targetGroundTruth =
      m_groundTruthGraph->target(groundTruthEdge);

  easy3d::vec3 sourceEstimatedPos = m_estimatedGraph->position(sourceEstimated);
  easy3d::vec3 targetEstimatedPos = m_estimatedGraph->position(targetEstimated);
  easy3d::vec3 sourceGroundTruthPos =
      m_groundTruthGraph->position(sourceGroundTruth);
  easy3d::vec3 targetGroundTruthPos =
      m_groundTruthGraph->position(targetGroundTruth);

  // Calculate direction vectors of both edges
  easy3d::vec3 dir1 = targetEstimatedPos - sourceEstimatedPos;
  easy3d::vec3 dir2 = targetGroundTruthPos - sourceGroundTruthPos;

  // Calculate lengths of both edges
  float len1 = dir1.length();
  float len2 = dir2.length();

  // Handle degenerate cases (when edges are points)
  if (len1 < 1e-6f || len2 < 1e-6f) {
    if (len1 < 1e-6f && len2 < 1e-6f) {
      return (sourceEstimatedPos - sourceGroundTruthPos).norm();
    }
    if (len1 < 1e-6f) {
      return pointToLineDistance(sourceEstimatedPos, sourceGroundTruthPos,
                                 targetGroundTruthPos);
    }
    return pointToLineDistance(sourceGroundTruthPos, sourceEstimatedPos,
                               targetEstimatedPos);
  }

  // Calculate unit direction vectors
  dir1 = dir1.normalize();
  dir2 = dir2.normalize();

  // Calculate normal vector between edges
  easy3d::vec3 n = easy3d::cross(dir1, dir2);
  float n_len = n.length();

  // Handle parallel edges
  if (n_len < 1e-6f) {
    // Calculate point-to-line distances
    float d1 = pointToLineDistance(sourceEstimatedPos, sourceGroundTruthPos,
                                   targetGroundTruthPos);
    float d2 = pointToLineDistance(targetEstimatedPos, sourceGroundTruthPos,
                                   targetGroundTruthPos);
    float d3 = pointToLineDistance(sourceGroundTruthPos, sourceEstimatedPos,
                                   targetEstimatedPos);
    float d4 = pointToLineDistance(targetGroundTruthPos, sourceEstimatedPos,
                                   targetEstimatedPos);
    return std::min({d1, d2, d3, d4});
  }

  // Calculate shortest distance between edges
  n = n.normalize();
  float d = std::abs(easy3d::dot(n, sourceEstimatedPos - sourceGroundTruthPos));

  // Check if closest points lie on the segments
  easy3d::vec3 p1 =
      sourceEstimatedPos +
      dir1 * easy3d::dot(dir1, sourceGroundTruthPos - sourceEstimatedPos);
  easy3d::vec3 p2 =
      sourceGroundTruthPos +
      dir2 * easy3d::dot(dir2, sourceEstimatedPos - sourceGroundTruthPos);

  // If closest points are not on segments, return minimum point-to-line
  // distance
  if (!isPointOnSegment(p1, sourceEstimatedPos, targetEstimatedPos) ||
      !isPointOnSegment(p2, sourceGroundTruthPos, targetGroundTruthPos)) {
    float d1 = pointToLineDistance(sourceEstimatedPos, sourceGroundTruthPos,
                                   targetGroundTruthPos);
    float d2 = pointToLineDistance(targetEstimatedPos, sourceGroundTruthPos,
                                   targetGroundTruthPos);
    float d3 = pointToLineDistance(sourceGroundTruthPos, sourceEstimatedPos,
                                   targetEstimatedPos);
    float d4 = pointToLineDistance(targetGroundTruthPos, sourceEstimatedPos,
                                   targetEstimatedPos);
    return std::min({d1, d2, d3, d4});
  }

  return d;
}

// Helper function: Calculate distance from a point to a line segment
float Analysis::pointToLineDistance(const easy3d::vec3& point,
                                    const easy3d::vec3& lineStart,
                                    const easy3d::vec3& lineEnd) {
  easy3d::vec3 line = lineEnd - lineStart;
  float lineLength = line.length();
  if (lineLength < 1e-6f) {
    return (point - lineStart).length();
  }

  easy3d::vec3 lineDir = line / lineLength;
  easy3d::vec3 pointToStart = point - lineStart;

  float t = easy3d::dot(pointToStart, lineDir);
  t = std::clamp(t, 0.0f, lineLength);

  easy3d::vec3 projection = lineStart + lineDir * t;
  return (point - projection).length();
}

// Helper function: Check if a point lies on a line segment
bool Analysis::isPointOnSegment(const easy3d::vec3& point,
                                const easy3d::vec3& segmentStart,
                                const easy3d::vec3& segmentEnd) {
  easy3d::vec3 segment = segmentEnd - segmentStart;
  easy3d::vec3 pointToStart = point - segmentStart;

  float t = easy3d::dot(pointToStart, segment) / segment.length2();
  return t >= 0.0f && t <= 1.0f;
}
}  // namespace myAnalysis
