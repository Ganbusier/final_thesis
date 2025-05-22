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

void Ransac3d::storeCylinderInfos() {}

}  // namespace ransac
