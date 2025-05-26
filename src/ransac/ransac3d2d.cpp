#include <easy3d/algo/point_cloud_normals.h>

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
  detectLines2d();
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

}  // namespace ransac
