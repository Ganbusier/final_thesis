#include <easy3d/algo/point_cloud_normals.h>

#include "ransac3d.h"

namespace ransac {
Ransac3d::Ransac3d(easy3d::PointCloud* pointCloud, const Params& params) {
  m_pointCloud = pointCloud;
  m_params.normal_threshold = params.normalThreshold;
  m_params.probability = params.probability;
  m_params.min_points = params.minPoints;
  m_params.epsilon = params.epsilon;
  m_params.cluster_epsilon = params.clusterEpsilon;
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
  m_ransac.detect(m_params);
  if (m_ransac.shapes().empty()) {
    LOG(INFO) << "No cylinders detected";
    return;
  }
  storeCylinders();
  storeCylinderInfos();
  storeLeftoverIndices();
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
    auto c = cylinder.axis().point(0);
    easy3d::vec3 direction = easy3d::vec3(d.x(), d.y(), d.z());
    easy3d::vec3 center = easy3d::vec3(c.x(), c.y(), c.z());
    const std::vector<size_t>& inlierIndices =
        cylinder.indices_of_assigned_points();

    // get bbox of inlier points
    easy3d::PointCloud* tempCloud = new easy3d::PointCloud;
    for (const auto& index : inlierIndices) {
      easy3d::PointCloud::Vertex v(index);
      easy3d::vec3 p = m_pointCloud->position(v);
      tempCloud->add_vertex(p);
    }
    const easy3d::Box3& bbox = tempCloud->bounding_box();
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
  for (const auto& v : m_pointCloud->vertices()) {
    m_leftoverIndices.push_back(v.idx());
  }
  for (const auto& cyInfo : m_cylinderInfos) {
    for (const auto& index : cyInfo.inlierIndices) {
      m_leftoverIndices.erase(std::remove(m_leftoverIndices.begin(),
                                          m_leftoverIndices.end(), index),
                              m_leftoverIndices.end());
    }
  }
}
}  // namespace ransac
