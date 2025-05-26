#include <easy3d/algo/point_cloud_normals.h>

#include <Eigen/Dense>

#include "ransac3d2d.h"

namespace ransac {
Ransac3d2d::Ransac3d2d(easy3d::PointCloud* pointCloud,
                       const Params_ransac3d2d& params) {
  m_pointCloud = pointCloud;
  m_ransac3dParams.normal_threshold = params.normalThreshold;
  m_ransac3dParams.probability = params.probability;
  m_ransac3dParams.min_points = params.minPoints;
  m_ransac3dParams.epsilon = params.epsilon;
  m_ransac3dParams.cluster_epsilon = params.clusterEpsilon;
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
  if (m_planes.empty()) {
    return;
  }
  detectLines2d();
  if (m_lines2d.empty()) {
    return;
  }
  lines2dToLines3d();
}

void Ransac3d2d::detectPlanes() {
  LOG(INFO) << "Detecting planes using RANSAC";
  m_ransac3d.set_input(m_pwnVector);
  m_ransac3d.add_shape_factory<Plane>();
  m_ransac3d.detect(m_ransac3dParams);
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
}

void Ransac3d2d::detectLines2d() {
  LOG(INFO) << "Detecting lines in 2D using RANSAC";
}

void Ransac3d2d::lines2dToLines3d() {
  LOG(INFO) << "Converting lines in 2D to lines in 3D";
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
  easy3d::vec2 direction(line.end.x - line.start.x, line.end.y - line.start.y);
  if (direction.norm() < 1e-6) {
    return {line};
  }
  direction.normalize();

  //projection
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
      
    }
    clusters.push_back(current_cluster);
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
    Line2d new_line = computeLineModel(cluster_points.front(), cluster_points.back());
    refineLineWithPCA(new_line, cluster_points);
    new_line.inlierIndices = indices;
    split_lines.push_back(new_line);
  }

  return split_lines.empty() ? std::vector{line} : split_lines;
}

void Ransac3d2d::storeLines2d() {}

void Ransac3d2d::storeLines3d() {}

void Ransac3d2d::storeLeftoverPoints() {}

void Ransac3d2d::saveLines3d(const std::string& filename) {}

void Ransac3d2d::saveLeftoverPoints(const std::string& filename) {}

}  // namespace ransac
