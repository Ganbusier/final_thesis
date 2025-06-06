#include <easy3d/algo/point_cloud_normals.h>
#include <easy3d/core/graph.h>
#include <easy3d/fileio/graph_io.h>

#include "ransac3d.h"

namespace ransac {
Ransac3d::Ransac3d(easy3d::PointCloud* pointCloud, const Params& params) {
  m_pointCloud = pointCloud;
  m_params = params;
  pointCloudToPwnVector();
}

void Ransac3d::pointCloudToPwnVector() {
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

void Ransac3d::detect() {
  LOG(INFO) << "Detecting cylinders using RANSAC";
  m_ransac.set_input(m_pwnVector);

  Cylinder::set_radiusRange(m_params.minRadius, m_params.maxRadius);

  Efficient_ransac::Parameters params;
  params.probability = m_params.probability;
  params.min_points = m_params.minPoints;
  params.epsilon = m_params.epsilon;
  params.cluster_epsilon = m_params.clusterEpsilon;
  params.normal_threshold = m_params.normalThreshold;

  m_ransac.add_shape_factory<Cylinder>();
  m_ransac.detect(params);
  if (m_ransac.shapes().empty()) {
    LOG(INFO) << "No cylinders detected";
    storeLeftoverIndices();
    storeLeftoverPoints();
    return;
  }
  auto shapes = m_ransac.shapes();
  for (const auto& shape : shapes) {
    if (Cylinder* cylinder = dynamic_cast<Cylinder*>(shape.get())) {
      m_cylinders.push_back(*cylinder);
    }
  }
  storeCylinders();
  storeCylinderInfos();
  storeLeftoverIndices();
  storeLeftoverPoints();
  LOG(INFO) << "Cylinders detected: " << m_cylinders.size();
  LOG(INFO) << "Leftover points: " << m_leftoverPoints.size();
}

void Ransac3d::storeCylinders() {
  m_cylinders.clear();
  auto shapes = m_ransac.shapes();
  for (const auto& shape : shapes) {
    if (Cylinder* cylinder = dynamic_cast<Cylinder*>(shape.get())) {
      m_cylinders.push_back(*cylinder);
    }
  }
}

void Ransac3d::storeCylinderInfos() {
  m_cylinderInfos.clear();
  for (const auto& cylinder : m_cylinders) {
    CylinderInfo info;
    auto d = cylinder.axis().to_vector();
    // auto c = cylinder.axis().point(0);
    easy3d::vec3 direction = easy3d::vec3(d.x(), d.y(), d.z());
    // easy3d::vec3 center = easy3d::vec3(c.x(), c.y(), c.z());
    const std::vector<size_t>& inlierIndices =
        cylinder.indices_of_assigned_points();

    // get bbox of inlier points
    easy3d::PointCloud* tempCloud = new easy3d::PointCloud;
    for (const auto& index : inlierIndices) {
      auto pwn = m_pwnVector[index];
      auto p = pwn.first;
      tempCloud->add_vertex(easy3d::vec3(p.x(), p.y(), p.z()));
    }
    const easy3d::Box3& bbox = tempCloud->bounding_box();
    auto center = bbox.center();
    info.start = center - direction * bbox.radius();
    info.end = center + direction * bbox.radius();
    info.radius = cylinder.radius();
    info.length = bbox.diagonal_length();

    for (const auto& index : inlierIndices) {
      info.inlierIndices.push_back(index);
    }

    m_cylinderInfos.push_back(info);
  }
}

void Ransac3d::storeLeftoverIndices() {
  m_leftoverIndices.clear();
  for (size_t i = 0; i < m_pwnVector.size(); ++i) {
    m_leftoverIndices.push_back(i);
  }
  if (m_cylinderInfos.empty()) {
    return;
  }
  for (const auto& cyInfo : m_cylinderInfos) {
    for (const auto& index : cyInfo.inlierIndices) {
      m_leftoverIndices.erase(std::remove(m_leftoverIndices.begin(),
                                          m_leftoverIndices.end(), index),
                              m_leftoverIndices.end());
    }
  }
}

void Ransac3d::storeLeftoverPoints() {
  m_leftoverPoints.clear();
  for (const auto& index : m_leftoverIndices) {
    auto pwn = m_pwnVector[index];
    auto p = pwn.first;
    m_leftoverPoints.push_back(easy3d::vec3(p.x(), p.y(), p.z()));
  }
}

void Ransac3d::saveCylinderInfos(const std::string& filename) {
  std::ofstream file(filename);
  file << "radius,length,start_x,start_y,start_z,end_x,end_y,end_z\n";
  for (const auto& cylinder : m_cylinderInfos) {
    file << cylinder.radius << "," << cylinder.length << ","
         << cylinder.start.x << "," << cylinder.start.y << ","
         << cylinder.start.z << "," << cylinder.end.x << ","
         << cylinder.end.y << "," << cylinder.end.z << "\n";
  }
  file.close();
  LOG(INFO) << "Cylinder infos saved to " << filename;
}

void Ransac3d::saveLeftoverPoints(const std::string& filename) {
  easy3d::PointCloud* leftoverCloud = new easy3d::PointCloud;
  for (const auto& point : m_leftoverPoints) {
    leftoverCloud->add_vertex(point);
  }
  easy3d::io::save_ply(filename, leftoverCloud, false);
  LOG(INFO) << "Leftover points saved to " << filename;
}

void Ransac3d::save3DLineSegments(const std::string& filename) {
  easy3d::Graph* graph = new easy3d::Graph;
  for (const auto& cylinder : m_cylinderInfos) {
    easy3d::Graph::Vertex v1 = graph->add_vertex(cylinder.start);
    easy3d::Graph::Vertex v2 = graph->add_vertex(cylinder.end);
    graph->add_edge(v1, v2);
  }
  easy3d::io::save_ply(filename, graph, false);
  LOG(INFO) << "3D line segments saved to " << filename;
}

}  // namespace ransac
