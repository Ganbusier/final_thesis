#include <easy3d/algo/point_cloud_normals.h>
#include <easy3d/core/graph.h>
#include <easy3d/fileio/graph_io.h>
#include <easy3d/fileio/point_cloud_io.h>

#include <Eigen/Dense>

#include "ransac3d2d.h"

namespace ransac {
Ransac3d2d::Ransac3d2d(easy3d::PointCloud* pointCloud,
                       const Params_ransac3d2d& params) {
  m_pointCloud = pointCloud;
  m_params = params;
  pointCloudToPwnVector();
}

void Ransac3d2d::pointCloudToPwnVector() {
  auto points = m_pointCloud->get_vertex_property<easy3d::vec3>("v:point");
  auto normals = m_pointCloud->get_vertex_property<easy3d::vec3>("v:normal");
  if (!normals) {
    LOG(INFO) << "Estimating normals using PCA, with k-neighbors = 16";
    bool estimate_normals =
        easy3d::PointCloudNormals::estimate(m_pointCloud, 16);
    if (!estimate_normals) {
      LOG(WARNING) << "No normals found or estimated for point cloud";
      return;
    }
    normals = m_pointCloud->get_vertex_property<easy3d::vec3>("v:normal");
  }

  LOG(INFO) << "Constructing point_with_normal_vector";
  m_pwnVector.clear();
  for (const auto& vertex : m_pointCloud->vertices()) {
    const easy3d::vec3 p = points[vertex];
    const easy3d::vec3 n = normals[vertex];
    m_pwnVector.emplace_back(Kernel::Point_3(p[0], p[1], p[2]),
                             Kernel::Vector_3(n[0], n[1], n[2]));
  }
  LOG(INFO) << "Point_with_normal_vector constructed";
}

void Ransac3d2d::detect() {
  detectPlanes();
  detectLines2d();
  lines2dToLines3d();

  // Store leftover indices first (these are needed by storeLeftoverIndices)
  storeLeftoverIndicesForPlane();
  storeLeftoverIndicesForLine();

  // Store combined leftover indices and points
  storeLeftoverIndices();
  storeLeftoverPoints();

  // Store leftover points for individual stages
  storeLeftoverPointsForPlane();
  storeLeftoverPointsForLine();
}

void Ransac3d2d::detectPlanes() {
  LOG(INFO) << "Detecting planes using RANSAC";
  m_ransac3d.set_input(m_pwnVector);
  m_ransac3d.add_shape_factory<Plane>();
  Efficient_ransac::Parameters params;
  params.normal_threshold = m_params.normalThreshold;
  params.probability = m_params.probability;
  params.min_points = m_params.minPoints;
  params.epsilon = m_params.epsilon;
  params.cluster_epsilon = m_params.clusterEpsilon;
  m_ransac3d.detect(params);
  if (m_ransac3d.shapes().empty()) {
    LOG(INFO) << "No planes detected";
    return;
  }
  auto shapes = m_ransac3d.shapes();
  for (const auto& shape : shapes) {
    if (Plane* plane = dynamic_cast<Plane*>(shape.get())) {
      m_planes.push_back(*plane);
    }
  }
  LOG(INFO) << "Detected " << m_planes.size() << " planes";
}

void Ransac3d2d::detectLines2d() {
  LOG(INFO) << "Detecting lines in 2D using RANSAC";

  // Handle the case when no planes are detected
  if (m_planes.empty()) {
    LOG(INFO) << "No planes detected, skipping line detection";
    return;
  }

  for (const auto& plane : m_planes) {
    std::vector<size_t> planeInlierIndices = plane.indices_of_assigned_points();
    std::vector<easy3d::vec2> points2d;
    for (const size_t index : planeInlierIndices) {
      auto p = m_pwnVector[index].first;
      auto p2d = plane.to_2d(p);
      points2d.push_back(easy3d::vec2(p2d.x(), p2d.y()));
    }

    // 2D RANSAC Line Detection
    std::vector<Line2d> lines;
    std::vector<size_t> remainingIndices(points2d.size());
    std::iota(remainingIndices.begin(), remainingIndices.end(), 0);
    size_t inlierThres = std::floor(points2d.size() * 0.1);
    size_t minModelSamples = 2;
    if (inlierThres <= minModelSamples) {
      // Add empty lines vector for this plane to maintain consistency
      m_lines2d.push_back(lines);
      continue;
    }

    while (inlierThres >= m_params.minInliers) {
      std::vector<Line2d> candidateLines;
      size_t mainIter = 0;
      // size_t ramdomSampleIter = 0;
      while (mainIter < m_params.maxIterations &&
             remainingIndices.size() > inlierThres &&
             inlierThres > m_params.minInliers) {
        // sample two points randomly
        std::vector<size_t> sampleIndices(2);
        std::sample(remainingIndices.begin(), remainingIndices.end(),
                    sampleIndices.begin(), 2, rng);
        const easy3d::vec2& p1 = points2d[sampleIndices[0]];
        const easy3d::vec2& p2 = points2d[sampleIndices[1]];

        // pass if points are overlapped
        if (std::hypot(p1.x - p2.x, p1.y - p2.y) < 1e-6) {
          // ramdomSampleIter++;
          mainIter++;
          continue;
        }
        // if (std::hypot(p1.x - p2.x, p1.y - p2.y) > 0.5) {
        //   ramdomSampleIter++;
        //   continue;
        // }

        // compute candidate line
        Line2d candidateLine = computeLineModel(p1, p2);

        // count inliers
        std::vector<size_t> candidateInliers;
        std::vector<easy3d::vec2> candidateInlierPoints;
        std::vector<std::pair<easy3d::vec2, int>> inlierData;
        for (size_t idx : remainingIndices) {
          const easy3d::vec2& p = points2d[idx];
          if (distanceToLine(p, candidateLine) < m_params.tolerance) {
            candidateInliers.push_back(idx);
            candidateInlierPoints.push_back(p);
            // Convert 2D index back to original point cloud index
            int originalIdx = static_cast<int>(planeInlierIndices[idx]);
            inlierData.emplace_back(p, originalIdx);
          }
        }
        if (candidateInliers.size() < inlierThres) {
          mainIter++;
          continue;
        }
        refineLineWithPCA(candidateLine, candidateInlierPoints);
        std::vector<Line2d> splitLines =
            splitLine(candidateLine, inlierData, m_params.splitDistanceThres);
        std::vector<size_t> validSplitLineInliers;
        for (const Line2d& l : splitLines) {
          if (l.inlierIndices.size() >= m_params.minInliers) {
            candidateLines.push_back(l);
            for (auto& idx : l.inlierIndices) {
              validSplitLineInliers.push_back(idx);
            }
          }
        }

        // remove inliers
        std::vector<size_t> newRemaining;
        std::set<size_t> inlierSet(validSplitLineInliers.begin(),
                                   validSplitLineInliers.end());
        for (size_t idx : remainingIndices) {
          if (!inlierSet.count(idx)) {
            newRemaining.push_back(idx);
          }
        }
        remainingIndices = newRemaining;
        mainIter++;
      }

      for (auto& l : candidateLines) {
        lines.push_back(l);
      }
      inlierThres = std::floor(inlierThres * 0.9);
    }

    m_lines2d.push_back(lines);
  }
}

void Ransac3d2d::lines2dToLines3d() {
  LOG(INFO) << "Converting lines in 2D to lines in 3D";

  // Handle the case when no lines are detected
  if (m_lines2d.empty()) {
    LOG(INFO) << "No 2D lines detected, skipping conversion to 3D";
    return;
  }

  for (size_t i = 0; i < m_planes.size(); ++i) {
    const auto& plane = m_planes[i];

    const std::vector<Line2d>& lines2d = m_lines2d[i];
    if (lines2d.empty()) {
      LOG(INFO) << "No lines found for plane " << i;
      continue;
    }

    std::vector<Line3d> lines3d;
    for (const auto& line2d : lines2d) {
      Line3d line3d;
      Point_3 p1 = plane.to_3d(Point_2(line2d.start.x, line2d.start.y));
      Point_3 p2 = plane.to_3d(Point_2(line2d.end.x, line2d.end.y));
      line3d.start = easy3d::vec3(p1.x(), p1.y(), p1.z());
      line3d.end = easy3d::vec3(p2.x(), p2.y(), p2.z());
      line3d.length = (line3d.end - line3d.start).length();
      line3d.inlierIndices = line2d.inlierIndices;
      lines3d.push_back(line3d);
    }
    m_lines3d.push_back(lines3d);
  }
}

Line2d Ransac3d2d::computeLineModel(const easy3d::vec2& p1,
                                    const easy3d::vec2& p2) {
  Line2d line;
  line.a = p1.y - p2.y;
  line.b = p2.x - p1.x;
  line.c = p1.x * p2.y - p2.x * p1.y;
  const double norm = std::hypot(line.a, line.b);
  if (norm < 1e-6) return line;
  line.a /= norm;
  line.b /= norm;
  line.c /= norm;
  line.start = p1;
  line.end = p2;
  return line;
}

double Ransac3d2d::distanceToLine(const easy3d::vec2& p, const Line2d& line) {
  return std::abs(line.a * p.x + line.b * p.y + line.c);
}

void Ransac3d2d::refineLineWithPCA(Line2d& line,
                                   const std::vector<easy3d::vec2>& inliers) {
  if (inliers.size() < 2) return;

  // mean point
  Eigen::Vector2d mean(0, 0);
  for (const auto& p : inliers) {
    mean += Eigen::Vector2d(p.x, p.y);
  }
  mean /= inliers.size();

  // covariance matrix
  Eigen::Matrix2d cov = Eigen::Matrix2d::Zero();
  for (const auto& p : inliers) {
    Eigen::Vector2d v(p.x - mean[0], p.y - mean[1]);
    cov += v * v.transpose();
  }
  cov /= inliers.size();

  // eigen decomposition
  Eigen::SelfAdjointEigenSolver<Eigen::Matrix2d> solver(cov);
  Eigen::Vector2d direction = solver.eigenvectors().col(1);

  // project all points to the main direction
  std::vector<double> projections;
  for (const auto& p : inliers) {
    Eigen::Vector2d v(p.x - mean[0], p.y - mean[1]);
    projections.push_back(v.dot(direction));
  }

  // find min and max of projected points
  auto minmax = std::minmax_element(projections.begin(), projections.end());
  double min_proj = *minmax.first;
  double max_proj = *minmax.second;

  // calculate end points
  line.start[0] = mean[0] + min_proj * direction[0];
  line.start[1] = mean[1] + min_proj * direction[1];
  line.end[0] = mean[0] + max_proj * direction[0];
  line.end[1] = mean[1] + max_proj * direction[1];
}

std::vector<Line2d> Ransac3d2d::splitLine(
    const Line2d& line,
    const std::vector<std::pair<easy3d::vec2, int>>& inlierData,
    double distanceThreshold) {
  /*
   * Check if a line needs to be split
   * If there is one pair of inliers that has a distance larger than the
   * threshold, then the line needs to be split. The inlier indices will be
   * stored in the line by this function.
   */
  easy3d::vec2 direction(line.end.x - line.start.x, line.end.y - line.start.y);
  if (direction.norm() < 1e-6) {
    return {line};
  }
  direction.normalize();

  // projection
  std::vector<std::tuple<double, easy3d::vec2, int>> projections;
  easy3d::vec2 start_vec(line.start.x, line.start.y);
  for (const auto& [p, idx] : inlierData) {
    double proj = easy3d::dot(p - start_vec, direction);
    projections.emplace_back(proj, p, idx);
  }

  // sort based on projection values
  std::sort(projections.begin(), projections.end(),
            [](const auto& a, const auto& b) {
              return std::get<0>(a) < std::get<0>(b);
            });

  // clustering
  std::vector<std::vector<std::tuple<double, easy3d::vec2, int>>> clusters;
  std::vector<std::tuple<double, easy3d::vec2, int>> current_cluster;
  if (!projections.empty()) {
    current_cluster.push_back(projections[0]);
    for (size_t i = 1; i < projections.size(); ++i) {
      double prev_proj = std::get<0>(projections[i - 1]);
      double curr_proj = std::get<0>(projections[i]);

      // Check if the distance between consecutive projections exceeds threshold
      if (std::abs(curr_proj - prev_proj) > distanceThreshold) {
        // Start a new cluster
        if (!current_cluster.empty()) {
          clusters.push_back(current_cluster);
          current_cluster.clear();
        }
      }
      current_cluster.push_back(projections[i]);
    }
    if (!current_cluster.empty()) {
      clusters.push_back(current_cluster);
    }
  }

  // generate new lines
  std::vector<Line2d> split_lines;
  for (const auto& cluster : clusters) {
    if (cluster.size() < 2) continue;

    // extract cluster points
    std::vector<easy3d::vec2> cluster_points;
    std::vector<int> indices;
    for (const auto& [proj, p, idx] : cluster) {
      cluster_points.push_back(p);
      indices.push_back(idx);
    }

    // calculate new lines
    Line2d new_line =
        computeLineModel(cluster_points.front(), cluster_points.back());
    refineLineWithPCA(new_line, cluster_points);
    new_line.inlierIndices = indices;
    split_lines.push_back(new_line);
  }

  return split_lines.empty() ? std::vector{line} : split_lines;
}

void Ransac3d2d::storeLeftoverIndices() {
  m_leftoverIndices.clear();

  // Use a boolean vector to mark assigned points - O(n) space, O(1) lookup
  std::vector<bool> assigned(m_pwnVector.size(), false);

  // Mark points assigned to planes
  for (const auto& plane : m_planes) {
    for (const auto& idx : plane.indices_of_assigned_points()) {
      if (idx < assigned.size()) {
        assigned[idx] = true;
      }
    }
  }

  // Mark points assigned to lines
  for (const auto& lines : m_lines2d) {
    for (const auto& line : lines) {
      for (const auto& idx : line.inlierIndices) {
        if (static_cast<size_t>(idx) < assigned.size()) {
          assigned[static_cast<size_t>(idx)] = true;
        }
      }
    }
  }

  // Collect unassigned indices in a single pass
  m_leftoverIndices.reserve(
      m_pwnVector.size());  // Pre-allocate to avoid reallocations
  for (size_t i = 0; i < assigned.size(); ++i) {
    if (!assigned[i]) {
      m_leftoverIndices.push_back(static_cast<int>(i));
    }
  }
}

void Ransac3d2d::storeLeftoverPoints() {
  m_leftoverPoints.clear();
  m_leftoverPoints.reserve(m_leftoverIndices.size());

  for (const auto& idx : m_leftoverIndices) {
    if (idx < m_pwnVector.size()) {
      auto p = m_pwnVector[idx].first;
      m_leftoverPoints.push_back(easy3d::vec3(p.x(), p.y(), p.z()));
    } else {
      LOG(ERROR) << "Leftover index out of range: " << idx;
    }
  }
}

void Ransac3d2d::saveLines3d(const std::string& filename) {
  if (m_lines3d.empty()) {
    LOG(WARNING) << "No 3D lines to save";
    return;
  }

  // Create a graph to store the lines
  easy3d::Graph* graph = new easy3d::Graph;

  for (size_t plane_idx = 0; plane_idx < m_lines3d.size(); ++plane_idx) {
    const auto& lines = m_lines3d[plane_idx];
    for (size_t line_idx = 0; line_idx < lines.size(); ++line_idx) {
      const auto& line = lines[line_idx];

      // Add vertices for line endpoints
      easy3d::Graph::Vertex v1 = graph->add_vertex(line.start);
      easy3d::Graph::Vertex v2 = graph->add_vertex(line.end);

      // Add edge connecting the vertices
      graph->add_edge(v1, v2);
    }
  }

  // Save the graph
  easy3d::io::save_ply(filename, graph, false);
  LOG(INFO) << "Lines saved to: " << filename;
}

void Ransac3d2d::saveLeftoverPoints(const std::string& filename) {
  if (m_leftoverPoints.empty()) {
    LOG(WARNING) << "No leftover points to save";
    return;
  }

  // Create a point cloud for leftover points
  auto leftoverCloud = new easy3d::PointCloud;

  for (const auto& point : m_leftoverPoints) {
    leftoverCloud->add_vertex(point);
  }

  // Save the point cloud
  easy3d::io::save_ply(filename, leftoverCloud, false);
  LOG(INFO) << "Leftover points saved to: " << filename;
}

void Ransac3d2d::storeLeftoverIndicesForPlane() {
  m_leftoverIndicesForPlane.clear();

  // Use a boolean vector to mark points assigned to planes
  std::vector<bool> assignedToPlane(m_pwnVector.size(), false);

  // handle the case that no planes are detected
  if (m_planes.empty()) {
    for (size_t i = 0; i < m_pwnVector.size(); ++i) {
      m_leftoverIndicesForPlane.push_back(static_cast<int>(i));
    }
    return;
  }

  // Mark points assigned to planes
  for (const auto& plane : m_planes) {
    for (const auto& idx : plane.indices_of_assigned_points()) {
      if (idx < assignedToPlane.size()) {
        assignedToPlane[idx] = true;
      }
    }
  }

  // Collect indices not assigned to any plane
  m_leftoverIndicesForPlane.reserve(m_pwnVector.size());
  for (size_t i = 0; i < assignedToPlane.size(); ++i) {
    if (!assignedToPlane[i]) {
      m_leftoverIndicesForPlane.push_back(static_cast<int>(i));
    }
  }

  LOG(INFO) << "Leftover indices after plane detection: "
            << m_leftoverIndicesForPlane.size();
}

void Ransac3d2d::storeLeftoverIndicesForLine() {
  m_leftoverIndicesForLine.clear();

  // Use a boolean vector to mark points assigned to lines
  std::vector<bool> assignedToLine(m_pwnVector.size(), false);

  // Mark points assigned to lines
  if (!m_lines2d.empty()) {
    for (const auto& lines : m_lines2d) {
      for (const auto& line : lines) {
        for (const auto& idx : line.inlierIndices) {
          if (static_cast<size_t>(idx) < assignedToLine.size()) {
            assignedToLine[static_cast<size_t>(idx)] = true;
          }
        }
      }
    }
  }

  // Collect ALL indices not assigned to any line (regardless of plane
  // assignment)
  m_leftoverIndicesForLine.reserve(m_pwnVector.size());
  for (size_t i = 0; i < assignedToLine.size(); ++i) {
    if (!assignedToLine[i]) {
      m_leftoverIndicesForLine.push_back(static_cast<int>(i));
    }
  }

  LOG(INFO) << "Leftover indices after line detection: "
            << m_leftoverIndicesForLine.size();
}

void Ransac3d2d::storeLeftoverPointsForPlane() {
  m_leftoverPointsForPlane.clear();
  m_leftoverPointsForPlane.reserve(m_leftoverIndicesForPlane.size());

  for (const auto& idx : m_leftoverIndicesForPlane) {
    if (idx < m_pwnVector.size()) {
      auto p = m_pwnVector[idx].first;
      m_leftoverPointsForPlane.push_back(easy3d::vec3(p.x(), p.y(), p.z()));
    }
  }
}

void Ransac3d2d::storeLeftoverPointsForLine() {
  m_leftoverPointsForLine.clear();
  m_leftoverPointsForLine.reserve(m_leftoverIndicesForLine.size());

  for (const auto& idx : m_leftoverIndicesForLine) {
    if (idx < m_pwnVector.size()) {
      auto p = m_pwnVector[idx].first;
      m_leftoverPointsForLine.push_back(easy3d::vec3(p.x(), p.y(), p.z()));
    }
  }
}

void Ransac3d2d::saveLeftoverPointsForPlane(const std::string& filename) {
  if (m_leftoverPointsForPlane.empty()) {
    LOG(WARNING) << "No leftover points for plane to save";
    return;
  }

  // Create a point cloud for leftover points

  auto leftoverCloud = new easy3d::PointCloud;

  for (const auto& point : m_leftoverPointsForPlane) {
    leftoverCloud->add_vertex(point);
  }

  // Save the point cloud
  easy3d::io::save_ply(filename, leftoverCloud, false);
  LOG(INFO) << "Leftover points for plane saved to: " << filename;
}

void Ransac3d2d::saveLeftoverPointsForLine(const std::string& filename) {
  if (m_leftoverPointsForLine.empty()) {
    LOG(WARNING) << "No leftover points for line to save";
    return;
  }

  // Create a point cloud for leftover points
  auto leftoverCloud = new easy3d::PointCloud;

  for (const auto& point : m_leftoverPointsForLine) {
    leftoverCloud->add_vertex(point);
  }

  // Save the point cloud
  easy3d::io::save_ply(filename, leftoverCloud, false);
  LOG(INFO) << "Leftover points for line saved to: " << filename;
}

}  // namespace ransac
