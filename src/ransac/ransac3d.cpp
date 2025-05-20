#include <easy3d/3rd_party/ransac/PointCloud.h>
#include <easy3d/algo/point_cloud_normals.h>

#include "ransac3d.h"

namespace ransac {

CustomCylinderRANSAC::CustomCylinderRANSAC() {}

CustomCylinderRANSAC::~CustomCylinderRANSAC() {}

void CustomCylinderRANSAC::detect(easy3d::PointCloud* cloud) {
  detected_cylinders_.clear();

  // set ransac options
  RansacShapeDetector::Options ransacOptions;
  ransacOptions.m_epsilon = epsilon_;
  ransacOptions.m_bitmapEpsilon = bitmap_resolution_;
  ransacOptions.m_normalThresh = normal_threshold_;
  ransacOptions.m_minSupport = min_support_;
  ransacOptions.m_probability = probability_;

  RansacShapeDetector detector(ransacOptions);

  // add custom cylinder constructor
  detector.Add(&cylinder_constructor_);

  // estimate normals
  easy3d::PointCloudNormals::estimate(cloud, 16);
  auto normals = cloud->get_vertex_property<easy3d::vec3>("v:normal");

  // convert point cloud format
  PointCloud pc;
  pc.resize(cloud->n_vertices());
  for (size_t i = 0; i < cloud->n_vertices(); ++i) {
    easy3d::PointCloud::Vertex v(i);
    const auto& p = cloud->position(v);
    const auto& n = normals[v];
    pc[i] = Vec3f(p.x, p.y, p.z);
    pc[i + cloud->n_vertices()] = Vec3f(n.x, n.y, n.z);
  }

  // detect
  MiscLib::Vector<std::pair<MiscLib::RefCountPtr<PrimitiveShape>, size_t>>
      shapes;
  detector.Detect(pc, 0, pc.size(), &shapes);

  // process detected results
  for (size_t i = 0; i < shapes.size(); ++i) {
    if (shapes[i].first->Identifier() == 2) {  // 2 is the identifier for cylinder
      CylinderPrimitiveShape* cylinder =
          static_cast<CylinderPrimitiveShape*>(shapes[i].first.operator->());
      detected_cylinders_.push_back(*cylinder);
    }
  }
}

}  // namespace ransac
