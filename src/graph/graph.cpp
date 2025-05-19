#include "graph.h"

namespace graph {
Graph::Graph(easy3d::PointCloud *pointCloud, int k, float maxEdgeLength)
    : m_pointCloud(pointCloud), m_k(k), m_maxEdgeLength(maxEdgeLength) {
  m_knnGraph = new easy3d::Graph;
  m_dtGraph = new easy3d::Graph;
  m_mixGraph = new easy3d::Graph;
  m_dualGraph = new easy3d::Graph;
}

Graph::~Graph() {}

easy3d::Graph *Graph::getKNNGraph() const { return m_knnGraph; }

easy3d::Graph *Graph::getDTGraph() const { return m_dtGraph; }

easy3d::Graph *Graph::getMixGraph() const { return m_mixGraph; }

easy3d::Graph *Graph::getDualGraph() const { return m_dualGraph; }

easy3d::PointCloud *Graph::getPointCloud() const { return m_pointCloud; }

void Graph::saveGraph(const std::string &filename, const easy3d::Graph *graph,
                      bool binary) const {
  easy3d::io::save_ply(filename, graph, binary);
}

void Graph::loadGraph(const std::string &filename, easy3d::Graph *graph) {
  easy3d::io::load_ply(filename, graph);
}

void Graph::buildKNNGraph() {
  LOG(INFO) << "Building KNN graph";
  m_knnGraph->clear();
  auto tree = easy3d::KdTreeSearch_ETH(m_pointCloud);
  std::map<int, easy3d::Graph::Vertex> vertexMap;
  std::set<std::pair<int, int>> addedEdges;

  // add points to graph
  for (const auto &v : m_pointCloud->vertices()) {
    easy3d::vec3 p = m_pointCloud->position(v);
    easy3d::Graph::Vertex gv = m_knnGraph->add_vertex(p);
    vertexMap[v.idx()] = gv;
  }

  // add edges to graph
  for (const auto &v : m_pointCloud->vertices()) {
    easy3d::vec3 p = m_pointCloud->position(v);
    std::vector<int> neighborIndices;
    std::vector<float> neighborSqrdDistances;
    tree.find_closest_k_points(p, m_k, neighborIndices, neighborSqrdDistances);
    for (const auto neighborIdx : neighborIndices) {
      if (neighborIdx == v.idx()) continue;
      int minIdx = std::min(v.idx(), neighborIdx);
      int maxIdx = std::max(v.idx(), neighborIdx);
      auto edgePair = std::make_pair(minIdx, maxIdx);
      addedEdges.insert(edgePair);
    }
  }

  for (const auto &edge : addedEdges) {
    m_knnGraph->add_edge(vertexMap[edge.first], vertexMap[edge.second]);
  }

  LOG(INFO) << "KNN graph built with " << m_knnGraph->n_vertices()
            << " vertices and " << m_knnGraph->n_edges() << " edges";
}

void Graph::buildDTGraph() {
  LOG(INFO) << "Building DT graph";
  m_dtGraph->clear();
  std::map<int, easy3d::Graph::Vertex> vertexMap;

  // add points to graph
  for (const auto &v : m_pointCloud->vertices()) {
    easy3d::vec3 p = m_pointCloud->position(v);
    easy3d::Graph::Vertex gv = m_dtGraph->add_vertex(p);
    vertexMap[v.idx()] = gv;
  }

  // build delaunay triangulation
  std::vector<easy3d::vec3> points;
  for (const auto &v : m_pointCloud->vertices()) {
    easy3d::vec3 p = m_pointCloud->position(v);
    points.push_back(p);
  }
  easy3d::Delaunay3 delaunay3d;
  delaunay3d.set_vertices(points);

  // extract edges from delaunay triangulation
  std::set<std::pair<int, int>> addedEdges;
  for (unsigned int i = 0; i < delaunay3d.nb_tets(); ++i) {
    for (int j = 0; j < 4; ++j) {
      for (int k = j + 1; k < 4; ++k) {
        int v1 = delaunay3d.tet_vertex(i, j);
        int v2 = delaunay3d.tet_vertex(i, k);
        int minIdx = std::min(v1, v2);
        int maxIdx = std::max(v1, v2);
        addedEdges.insert(std::make_pair(minIdx, maxIdx));
      }
    }
  }

  // add edges to graph
  for (const auto &edge : addedEdges) {
    m_dtGraph->add_edge(vertexMap[edge.first], vertexMap[edge.second]);
  }

  LOG(INFO) << "DT graph built with " << m_dtGraph->n_vertices()
            << " vertices and " << m_dtGraph->n_edges() << " edges";
}

void Graph::buildMixGraph() {
  LOG(INFO) << "Building mix graph";
  m_mixGraph->clear();

  if (m_knnGraph->empty()) {
    LOG(WARNING) << "KNN graph is empty, building KNN graph";
    buildKNNGraph();
  }
  if (m_dtGraph->empty()) {
    LOG(WARNING) << "DT graph is empty, building DT graph";
    buildDTGraph();
  }

  m_mixGraph = m_knnGraph;

  std::set<std::pair<int, int>> addedEdges;
  for (const auto &edge : m_knnGraph->edges()) {
    auto source = m_knnGraph->source(edge);
    auto target = m_knnGraph->target(edge);
    int minIdx = std::min(source.idx(), target.idx());
    int maxIdx = std::max(source.idx(), target.idx());
    addedEdges.insert(std::make_pair(minIdx, maxIdx));
  }

  unsigned int dtEdgesCount = 0;
  for (const auto &edge : m_dtGraph->edges()) {
    auto source = m_dtGraph->source(edge);
    auto target = m_dtGraph->target(edge);
    auto start = m_dtGraph->position(source);
    auto end = m_dtGraph->position(target);
    float edgeLength = (end - start).length();
    if (edgeLength > m_maxEdgeLength) {
      continue;
    }
    int minIdx = std::min(source.idx(), target.idx());
    int maxIdx = std::max(source.idx(), target.idx());
    auto edgePair = std::make_pair(minIdx, maxIdx);

    if (addedEdges.find(edgePair) == addedEdges.end()) {
      m_mixGraph->add_edge(source, target);
      addedEdges.insert(edgePair);
      dtEdgesCount++;
    }
  }

  LOG(INFO) << "Mix graph built with " << m_mixGraph->n_vertices()
            << " vertices and " << m_mixGraph->n_edges() << " edges, "
            << dtEdgesCount << " edges from DT graph";
}

void Graph::buildDualGraph(const easy3d::Graph *graph) {
  LOG(INFO) << "Building dual graph";
  m_dualGraph->clear();

  std::map<int, easy3d::Graph::Vertex> EdgeToVertexMap;
  std::map<int, std::vector<int>> VertexToAdjacentEdgesMap;
  std::set<std::pair<int, int>> addedEdges;

  // add edges from graph as vertices to dual graph
  for (const auto &edge : graph->edges()) {
    auto sourcePos = graph->position(graph->source(edge));
    auto targetPos = graph->position(graph->target(edge));
    auto midPos = (sourcePos + targetPos) * 0.5f;
    auto gv = m_dualGraph->add_vertex(midPos);
    EdgeToVertexMap[edge.idx()] = gv;
  }

  // build map from vertices to adjacent edges
  for (const auto &edge : graph->edges()) {
    auto source = graph->source(edge);
    auto target = graph->target(edge);
    VertexToAdjacentEdgesMap[source.idx()].push_back(edge.idx());
    VertexToAdjacentEdgesMap[target.idx()].push_back(edge.idx());
  }

  // create edges between dual graph vertices
  for (const auto &[vertexIdx, adjacentEdges] : VertexToAdjacentEdgesMap) {
    for (const auto &edgeIdx1 : adjacentEdges) {
      for (const auto &edgeIdx2 : adjacentEdges) {
        if (edgeIdx1 == edgeIdx2) continue;

        int minIdx = std::min(edgeIdx1, edgeIdx2);
        int maxIdx = std::max(edgeIdx1, edgeIdx2);
        auto edgePair = std::make_pair(minIdx, maxIdx);
        if (addedEdges.find(edgePair) != addedEdges.end()) continue;

        auto dv1 = EdgeToVertexMap[edgeIdx1];
        auto dv2 = EdgeToVertexMap[edgeIdx2];
        m_dualGraph->add_edge(dv1, dv2);
        addedEdges.insert(edgePair);
      }
    }
  }

  LOG(INFO) << "Dual graph built with " << m_dualGraph->n_vertices()
            << " vertices and " << m_dualGraph->n_edges() << " edges";
}

}  // namespace graph
