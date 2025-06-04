#include "energyMinimization.h"

namespace energyMinimization {

EnergyMinimization::EnergyMinimization(int num_nodes, int num_neighborPairs,
                                       easy3d::Graph *graph,
                                       easy3d::PointCloud *pointCloud,
                                       const EnergyMinimizationParams &params)
    : m_numNodes(num_nodes),
      m_numNeighborPairs(num_neighborPairs),
      m_graph(graph),
      m_pointCloud(pointCloud) {
  m_gc = new GCoptimizationGeneralGraph(m_numNodes, m_numLabels);
  m_dataTerm = std::vector<int>(m_numNodes, m_scaleFactor);
  m_inlierProbTerm = std::vector<float>(m_numNodes, 0.0f);
  m_edgeLengthTerm = std::vector<float>(m_numNodes, 1.0f);
  m_preservedGraph = new easy3d::Graph;
  m_removedGraph = new easy3d::Graph;
  m_params = params;
}

EnergyMinimization::~EnergyMinimization() {}

void EnergyMinimization::setDataTerm() {
  LOG(INFO) << "Setting data term";
  computeDataTerm();
  for (size_t i = 0; i < m_numNodes; i++) {
    m_gc->setDataCost(i, 0, m_scaleFactor - m_dataTerm[i]);  // 0: removed
    m_gc->setDataCost(i, 1, m_dataTerm[i]);                  // 1: preserved
  }
}

void EnergyMinimization::setSmoothnessTerm() {
  LOG(INFO) << "Setting smoothness term";
  computeNeighborPairWeights();
  for (const auto &ng : m_neighborPairs) {
    int scaledWeight =
        static_cast<int>(std::floor(ng.weight * m_scaleFactor * m_params.lambda));
    m_gc->setNeighbors(ng.node1_idx, ng.node2_idx, scaledWeight);
  }
  m_gc->setSmoothCost(m_V);
}

void EnergyMinimization::optimize() {
  LOG(INFO) << "Begin optimization";
  LOG(INFO) << "Number of nodes: " << m_numNodes;
  LOG(INFO) << "Number of labels: " << m_numLabels;
  LOG(INFO) << "Number of neighbor pairs: " << m_numNeighborPairs;
  LOG(INFO) << "Energy before optimization: " << m_gc->compute_energy()
            << "; Data energy: " << m_gc->giveDataEnergy()
            << "; Smoothness energy: " << m_gc->giveSmoothEnergy();

  m_gc->expansion(99);

  LOG(INFO) << "Energy after optimization: " << m_gc->compute_energy()
            << "; Data energy: " << m_gc->giveDataEnergy()
            << "; Smoothness energy: " << m_gc->giveSmoothEnergy();
}

void EnergyMinimization::computeDataTerm() {
  LOG(INFO) << "Computing data term";
  computeInlierProbTerm();
  computeEdgeLengthTerm();
  for (int i = 0; i < m_numNodes; i++) {
    m_dataTerm[i] = static_cast<int>(
        std::floor(m_inlierProbTerm[i] * m_edgeLengthTerm[i] * m_scaleFactor));
  }
}

void EnergyMinimization::computeInlierProbTerm() {
  LOG(INFO) << "Computing inlier probability term";
  auto tree = easy3d::KdTreeSearch_ETH(m_pointCloud);

  float meanSpacing = 0.0f;
  int pointCount = 0;
  for (const auto &point : m_pointCloud->points()) {
    std::vector<int> neighborIndices;
    std::vector<float> neighborSqrdDistances;
    tree.find_closest_k_points(point, 2, neighborIndices,
                               neighborSqrdDistances);
    if (neighborIndices.size() > 1) {
      meanSpacing += std::sqrt(neighborSqrdDistances[1]);
      pointCount++;
    }
  }
  if (pointCount > 0) {
    meanSpacing /= pointCount;
  } else {
    meanSpacing = 0.1f;
  }
  LOG(INFO) << "Mean spacing: " << meanSpacing;

  for (const auto &e : m_graph->edges()) {
    // get source and target of the edge
    auto source = m_graph->source(e);
    auto target = m_graph->target(e);
    auto sourcePos = m_graph->position(source);
    auto targetPos = m_graph->position(target);
    auto direction = (targetPos - sourcePos).normalize();

    // extend the source and target
    auto sourcePos_extended =
        sourcePos - direction * meanSpacing * m_params.extendFactor;
    auto targetPos_extended =
        targetPos + direction * meanSpacing * m_params.extendFactor;

    // find the inliers in the extended cylinder
    std::vector<int> inlierIndices;
    std::vector<float> inlierSqrdDistances;
    tree.find_points_in_cylinder(sourcePos_extended, targetPos_extended,
                                 m_params.inlierSearchRadius, inlierIndices,
                                 inlierSqrdDistances);
    if (inlierIndices.empty() || inlierSqrdDistances.empty()) {
      continue;
    }

    // filter out the source and target
    std::vector<float> inlierSqrdDistances_filtered;
    for (size_t i = 0; i < inlierIndices.size(); i++) {
      if (inlierIndices[i] != source.idx() &&
          inlierIndices[i] != target.idx()) {
        inlierSqrdDistances_filtered.push_back(inlierSqrdDistances[i]);
      }
    }
    if (inlierSqrdDistances_filtered.empty()) continue;

    // compute the inlier probabilities of all inliers
    std::vector<float> inlierProbabilities;
    for (const float sqrdDistance : inlierSqrdDistances_filtered) {
      float distance = std::sqrt(sqrdDistance);
      float radius = m_params.inlierSearchRadius;

      // compute the sigma_squared
      float epsilon = 1e-6f;
      float sigmaSqrd = -(radius * radius) / (2.0f * std::log(epsilon));

      float inlierProbability =
          std::exp(-distance * distance / (2.0f * sigmaSqrd));
      inlierProbabilities.push_back(inlierProbability);
    }

    // use the weighted average of the inlier probabilities as the inlier
    // probability term for the edge
    float sumOfWeights = 0.0f;
    float weightedSum = 0.0f;
    for (const float inlierProb : inlierProbabilities) {
      float weight =
          inlierProb;  // use the inlier probability itself as the weight
      sumOfWeights += weight;
      weightedSum += weight * inlierProb;
    }
    float inlierProbTerm =
        (sumOfWeights > 0.0f) ? (weightedSum / sumOfWeights) : 0.0f;
    m_inlierProbTerm[e.idx()] = 1.0f - inlierProbTerm;
  }
}

void EnergyMinimization::computeEdgeLengthTerm() {
  LOG(INFO) << "Computing edge length term";
  for (const auto &e : m_graph->edges()) {
    auto source = m_graph->source(e);
    auto target = m_graph->target(e);
    auto sourcePos = m_graph->position(source);
    auto targetPos = m_graph->position(target);
    auto direction = (targetPos - sourcePos).normalize();
    float edgeLength = (targetPos - sourcePos).length();
    float edgeFinalLength = edgeLength;

    std::queue<easy3d::Graph::Edge> edgeQueue;
    std::unordered_set<int> visitedEdges;
    edgeQueue.push(e);
    visitedEdges.insert(e.idx());

    while (!edgeQueue.empty()) {
      auto currentEdge = edgeQueue.front();
      edgeQueue.pop();
      auto currentSource = m_graph->source(currentEdge);
      auto currentTarget = m_graph->target(currentEdge);
      auto currentSourcePos = m_graph->position(currentSource);
      auto currentTargetPos = m_graph->position(currentTarget);
      auto currentDirection = (currentTargetPos - currentSourcePos).normalize();
      auto currentEdgeLength = (currentTargetPos - currentSourcePos).length();

      auto processVertex = [&](easy3d::Graph::Vertex v) {
        float minAngleInDegrees = m_params.minAngleInDegrees;
        easy3d::Graph::Edge bestEdge;
        for (const auto &neighborEdge : m_graph->edges(v)) {
          if (visitedEdges.count(neighborEdge.idx())) continue;
          visitedEdges.insert(neighborEdge.idx());
          auto neighborSource = m_graph->source(neighborEdge);
          auto neighborTarget = m_graph->target(neighborEdge);
          auto neighborSourcePos = m_graph->position(neighborSource);
          auto neighborTargetPos = m_graph->position(neighborTarget);
          auto neighborDirection =
              (neighborTargetPos - neighborSourcePos).normalize();
          auto neighborEdgeLength =
              (neighborTargetPos - neighborSourcePos).length();

          float cosine = easy3d::dot(currentDirection, neighborDirection);
          float angle =
              std::acos(std::clamp(cosine, -1.0f, 1.0f)) * 180.0f / M_PI;
          if (angle < minAngleInDegrees) {
            minAngleInDegrees = angle;
            bestEdge = neighborEdge;
          }
        }
        if (bestEdge.idx() != -1) {
          edgeQueue.push(bestEdge);
          auto bestEdgeSource = m_graph->source(bestEdge);
          auto bestEdgeTarget = m_graph->target(bestEdge);
          auto bestEdgeSourcePos = m_graph->position(bestEdgeSource);
          auto bestEdgeTargetPos = m_graph->position(bestEdgeTarget);
          auto bestEdgeDirection =
              (bestEdgeTargetPos - bestEdgeSourcePos).normalize();
          auto bestEdgeLength =
              (bestEdgeTargetPos - bestEdgeSourcePos).length();
          edgeFinalLength += bestEdgeLength;
        }
      };

      processVertex(currentSource);
      processVertex(currentTarget);
    }

    m_edgeLengthTerm[e.idx()] = edgeLength / edgeFinalLength;
  }
}

void EnergyMinimization::computeNeighborPairWeights() {
  LOG(INFO) << "Computing neighbor pair weights";
  std::set<std::pair<int, int>> uniqueNeighborPairs;

  for (const auto &e : m_graph->edges()) {
    auto source = m_graph->source(e);
    auto target = m_graph->target(e);
    auto sourcePos = m_graph->position(source);
    auto targetPos = m_graph->position(target);
    auto direction = (targetPos - sourcePos).normalize();

    auto processVertex = [&](easy3d::Graph::Vertex v) {
      for (const auto &neighborEdge : m_graph->edges(v)) {
        if (neighborEdge.idx() == e.idx()) continue;

        // avoid duplicate neighbor pairs
        int minIdx = std::min(e.idx(), neighborEdge.idx());
        int maxIdx = std::max(e.idx(), neighborEdge.idx());

        auto neighborPair = std::make_pair(minIdx, maxIdx);
        if (uniqueNeighborPairs.find(neighborPair) != uniqueNeighborPairs.end())
          continue;

        uniqueNeighborPairs.insert(neighborPair);
        auto neighborSource = m_graph->source(neighborEdge);
        auto neighborTarget = m_graph->target(neighborEdge);
        auto neighborSourcePos = m_graph->position(neighborSource);
        auto neighborTargetPos = m_graph->position(neighborTarget);
        auto neighborDirection =
            (neighborTargetPos - neighborSourcePos).normalize();

        // Calculate weight based on angle between edges
        float cosine = easy3d::dot(direction, neighborDirection);
        cosine = std::max(-1.0f, std::min(1.0f, cosine));
        float weight = std::pow(cosine, 10.0f);

        // Add to neighbor pairs
        m_neighborPairs.push_back({e.idx(), neighborEdge.idx(), weight});
      }
    };

    processVertex(source);
    processVertex(target);
  }
}

void EnergyMinimization::getResults() {
  std::unordered_map<int, easy3d::Graph::Vertex> preservedVertexMap;
  std::unordered_map<int, easy3d::Graph::Vertex> removedVertexMap;

  int preservedEdgeCount = 0;
  int removedEdgeCount = 0;

  for (const auto &e : m_graph->edges()) {
    auto source = m_graph->source(e);
    auto target = m_graph->target(e);

    if (!source.is_valid() || !target.is_valid()) {
      LOG(ERROR) << "Invalid vertex found for edge " << e.idx();
      continue;
    }

    auto sourcePos = m_graph->position(source);
    auto targetPos = m_graph->position(target);

    int label = m_gc->whatLabel(e.idx());

    if (label == 1) {
      easy3d::Graph::Vertex newSource, newTarget;

      if (preservedVertexMap.find(source.idx()) == preservedVertexMap.end()) {
        newSource = m_preservedGraph->add_vertex(sourcePos);
        preservedVertexMap[source.idx()] = newSource;
      } else {
        newSource = preservedVertexMap[source.idx()];
      }

      if (preservedVertexMap.find(target.idx()) == preservedVertexMap.end()) {
        newTarget = m_preservedGraph->add_vertex(targetPos);
        preservedVertexMap[target.idx()] = newTarget;
      } else {
        newTarget = preservedVertexMap[target.idx()];
      }

      m_preservedGraph->add_edge(newSource, newTarget);
      preservedEdgeCount++;
    } else if (label == 0) {
      easy3d::Graph::Vertex newSource, newTarget;

      if (removedVertexMap.find(source.idx()) == removedVertexMap.end()) {
        newSource = m_removedGraph->add_vertex(sourcePos);
        removedVertexMap[source.idx()] = newSource;
      } else {
        newSource = removedVertexMap[source.idx()];
      }

      if (removedVertexMap.find(target.idx()) == removedVertexMap.end()) {
        newTarget = m_removedGraph->add_vertex(targetPos);
        removedVertexMap[target.idx()] = newTarget;
      } else {
        newTarget = removedVertexMap[target.idx()];
      }

      m_removedGraph->add_edge(newSource, newTarget);
      removedEdgeCount++;
    } else {
      LOG(ERROR) << "Error: invalid label " << label << " for edge " << e.idx();
    }
  }

  LOG(INFO) << "Processed " << preservedEdgeCount << " preserved edges and " 
            << removedEdgeCount << " removed edges";
}

void EnergyMinimization::saveResults(const std::string &preservedFilename,
                                     const std::string &removedFilename) {
  LOG(INFO) << "Saving results to " << preservedFilename << " and "
            << removedFilename;
  easy3d::io::save_ply(preservedFilename, m_preservedGraph, false);
  easy3d::io::save_ply(removedFilename, m_removedGraph, false);
  LOG(INFO) << "Results saved to " << preservedFilename << " and "
            << removedFilename;
}

}  // namespace energyMinimization
