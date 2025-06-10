#include <algorithm>
#include <cmath>
#include <limits>
#include <numeric>
#include <set>

#include "analysis.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace myAnalysis {
void Analysis::analyze() {
  matchEdges();
  distanceAnalysis();
  angleAnalysis();
  calculateStatistics();
}

void Analysis::matchEdges() {
  // Clear previous matching results
  m_matchedEdges.clear();
  m_unmatchedEstimatedEdges.clear();
  m_unmatchedGroundTruthEdges.clear();

  // Distance threshold
  const float maxDistance = 0.1f;

  // Track which edges have been matched
  std::set<easy3d::Graph::Edge> matchedEstimatedEdges;
  std::set<easy3d::Graph::Edge> matchedGroundTruthEdges;

  // Iterate through each edge in the estimated graph
  for (auto estimatedEdge : m_estimatedGraph->edges()) {
    float minDistance = std::numeric_limits<float>::max();
    easy3d::Graph::Edge bestMatchEdge;
    bool foundMatch = false;

    // Iterate through each edge in the ground truth graph to find the nearest
    // edge
    for (auto groundTruthEdge : m_groundTruthGraph->edges()) {
      float distance = distanceBetweenTwoEdges(estimatedEdge, groundTruthEdge);

      if (distance < minDistance) {
        minDistance = distance;
        bestMatchEdge = groundTruthEdge;
        foundMatch = true;
      }
    }

    // Save the matching result if found match and distance is within threshold
    if (foundMatch && minDistance <= maxDistance) {
      m_matchedEdges[bestMatchEdge].push_back(estimatedEdge);
      matchedEstimatedEdges.insert(estimatedEdge);
      matchedGroundTruthEdges.insert(bestMatchEdge);
    }
  }

  // Find unmatched estimated edges
  for (auto estimatedEdge : m_estimatedGraph->edges()) {
    if (matchedEstimatedEdges.find(estimatedEdge) ==
        matchedEstimatedEdges.end()) {
      m_unmatchedEstimatedEdges.push_back(estimatedEdge);
    }
  }

  // Find unmatched ground truth edges
  for (auto groundTruthEdge : m_groundTruthGraph->edges()) {
    if (matchedGroundTruthEdges.find(groundTruthEdge) ==
        matchedGroundTruthEdges.end()) {
      m_unmatchedGroundTruthEdges.push_back(groundTruthEdge);
    }
  }
}

void Analysis::distanceAnalysis() {
  // Clear previous results
  m_meanDistances.clear();

  // Calculate average distance for each ground truth edge
  for (const auto& group : m_matchedEdges) {
    easy3d::Graph::Edge groundTruthEdge = group.first;
    const std::vector<easy3d::Graph::Edge>& estimatedEdges = group.second;

    float totalDistance = 0.0f;
    for (const auto& estimatedEdge : estimatedEdges) {
      totalDistance += distanceBetweenTwoEdges(estimatedEdge, groundTruthEdge);
    }

    float averageDistance = totalDistance / estimatedEdges.size();
    m_meanDistances.push_back(averageDistance);
  }
}

void Analysis::angleAnalysis() {
  // Clear previous results
  m_meanAngles.clear();

  // Calculate average angle for each ground truth edge
  for (const auto& group : m_matchedEdges) {
    easy3d::Graph::Edge groundTruthEdge = group.first;
    const std::vector<easy3d::Graph::Edge>& estimatedEdges = group.second;

    float totalAngle = 0.0f;
    for (const auto& estimatedEdge : estimatedEdges) {
      totalAngle += angleBetweenTwoEdges(estimatedEdge, groundTruthEdge);
    }

    float averageAngle = totalAngle / estimatedEdges.size();
    m_meanAngles.push_back(averageAngle);
  }
}

void Analysis::calculateStatistics() {
  // Clear previous results
  m_analysisResults.minDistance = -1.0f;
  m_analysisResults.maxDistance = -1.0f;
  m_analysisResults.minAngle = -1.0f;
  m_analysisResults.maxAngle = -1.0f;
  m_analysisResults.meanDistance = -1.0f;
  m_analysisResults.meanAngle = -1.0f;
  m_analysisResults.medianDistance = -1.0f;
  m_analysisResults.medianAngle = -1.0f;
  m_analysisResults.stdDistance = -1.0f;
  m_analysisResults.stdAngle = -1.0f;
  m_analysisResults.RMSEofMeanDistances = -1.0f;
  m_analysisResults.RMSEofMeanAngles = -1.0f;
  m_analysisResults.unmatchedEstimatedEdgesCount = 0;
  m_analysisResults.unmatchedGroundTruthEdgesCount = 0;

  // Calculate min, max, mean, median, std
  if (m_meanDistances.empty() || m_meanAngles.empty()) {
    // LOG(WARNING) << "Mean distances or angles are empty";
    return;
  }
  
  std::vector<float> sortedDistances = m_meanDistances;
  std::vector<float> sortedAngles = m_meanAngles;
  std::sort(sortedDistances.begin(), sortedDistances.end());
  std::sort(sortedAngles.begin(), sortedAngles.end());

  // Min and Max
  m_analysisResults.minDistance = sortedDistances.front();
  m_analysisResults.maxDistance = sortedDistances.back();
  m_analysisResults.minAngle = sortedAngles.front();
  m_analysisResults.maxAngle = sortedAngles.back();
  
  // Mean
  m_analysisResults.meanDistance =
      std::accumulate(sortedDistances.begin(), sortedDistances.end(), 0.0f) /
      sortedDistances.size();
  m_analysisResults.meanAngle =
      std::accumulate(sortedAngles.begin(), sortedAngles.end(), 0.0f) /
      sortedAngles.size();
      
  // Median (handle even/odd cases)
  if (sortedDistances.size() % 2 == 0) {
    m_analysisResults.medianDistance = 
        (sortedDistances[sortedDistances.size() / 2 - 1] + 
         sortedDistances[sortedDistances.size() / 2]) / 2.0f;
  } else {
    m_analysisResults.medianDistance = sortedDistances[sortedDistances.size() / 2];
  }
  
  if (sortedAngles.size() % 2 == 0) {
    m_analysisResults.medianAngle = 
        (sortedAngles[sortedAngles.size() / 2 - 1] + 
         sortedAngles[sortedAngles.size() / 2]) / 2.0f;
  } else {
    m_analysisResults.medianAngle = sortedAngles[sortedAngles.size() / 2];
  }
  
  // Standard deviation (properly calculated)
  float sumSquaredDiffDistances = 0.0f;
  float sumSquaredDiffAngles = 0.0f;
  
  for (const auto& distance : m_meanDistances) {
    float diff = distance - m_analysisResults.meanDistance;
    sumSquaredDiffDistances += diff * diff;
  }
  
  for (const auto& angle : m_meanAngles) {
    float diff = angle - m_analysisResults.meanAngle;
    sumSquaredDiffAngles += diff * diff;
  }
  
  m_analysisResults.stdDistance = 
      std::sqrt(sumSquaredDiffDistances / m_meanDistances.size());
  m_analysisResults.stdAngle = 
      std::sqrt(sumSquaredDiffAngles / m_meanAngles.size());

  /*
   * RMSE = sqrt(1/n * sum(x_i^2))
   * x_i: mean distance or angle difference between a ground truth edge and its
   *      matched estimated edges. Ideal value is 0 (perfect match), so we
   *      calculate RMSE against zero.
   *
   * n: number of ground truth edges with matches.
   */

  float sumSquaredDistances = 0.0f;
  float sumSquaredAngles = 0.0f;

  // Calculate sum of squared distances (ideal value is 0)
  for (const auto& distance : m_meanDistances) {
    sumSquaredDistances += distance * distance;
  }

  // Calculate sum of squared angles (ideal value is 0)
  for (const auto& angle : m_meanAngles) {
    sumSquaredAngles += angle * angle;
  }

  // Calculate RMSE
  m_analysisResults.RMSEofMeanDistances =
      std::sqrt(sumSquaredDistances / m_meanDistances.size());
  m_analysisResults.RMSEofMeanAngles =
      std::sqrt(sumSquaredAngles / m_meanAngles.size());
      
  // Calculate unmatched edges count
  m_analysisResults.unmatchedEstimatedEdgesCount = 
      static_cast<int>(m_unmatchedEstimatedEdges.size());
  m_analysisResults.unmatchedGroundTruthEdgesCount = 
      static_cast<int>(m_unmatchedGroundTruthEdges.size());
}

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
  // Take absolute value to ignore direction - always return acute angle (0° to 90°)
  float angle = std::acos(std::clamp(std::abs(cosine), 0.0f, 1.0f)) * 180.0f / M_PI;
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
