#include <easy3d/3rd_party/ransac/PointCloud.h>
#include <easy3d/algo/point_cloud_normals.h>

#include "ransac3d.h"

namespace ransac {

CustomCylinderRANSAC::CustomCylinderRANSAC() {}

CustomCylinderRANSAC::~CustomCylinderRANSAC() {}

void CustomCylinderRANSAC::detect(easy3d::PointCloud* cloud) {
  m_cylinders.clear();

  // set ransac options
  RansacShapeDetector::Options ransacOptions;
  ransacOptions.m_epsilon = m_epsilon;
  ransacOptions.m_bitmapEpsilon = m_bitmapResolution;
  ransacOptions.m_normalThresh = m_normalThreshold;
  ransacOptions.m_minSupport = m_minSupport;
  ransacOptions.m_probability = m_probability;

  RansacShapeDetector detector(ransacOptions);

  // add custom cylinder constructor
  detector.Add(&m_cylinderConstructor);

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

  PointCloud::reverse_iterator start = pc.rbegin();
  MiscLib::Vector<std::pair<MiscLib::RefCountPtr<PrimitiveShape>, size_t>>::const_iterator shape_itr = shapes.begin();

  // process detected results
  for (; shape_itr != shapes.end(); ++shape_itr) {
    const PrimitiveShape* primitive = shape_itr->first;
    if (primitive->Identifier() == 2) {  // 2 is the identifier for cylinder
      const Cylinder& cPrim =
          dynamic_cast<const CylinderPrimitiveShape*>(primitive)->Internal();
      const Vec3f& axisPos = cPrim.AxisPosition();
      const Vec3f& axisDir = cPrim.AxisDirection();
      float radius = cPrim.Radius();
      size_t numInliers = shape_itr->second;

      // get cylinder parameters
      CustomCylinder cylinder;

      // get inlier indices
      std::vector<int> vts(numInliers);
      easy3d::PointCloud* tempCloud = new easy3d::PointCloud;
      PointCloud::reverse_iterator point_itr = start;
      for (size_t idx = 0; idx < numInliers; ++idx) {
        vts[idx] = static_cast<int>(point_itr->index);
        ++point_itr;
      }
      start = point_itr;

      // add inlier indices
      for (auto id : vts) {
        const easy3d::PointCloud::Vertex v(id);
        cylinder.inlierIndices.push_back(id);
        tempCloud->add_vertex(cloud->position(v));
      }

      // get the rest of the cylinder parameters
      easy3d::vec3 center = easy3d::vec3(axisPos[0], axisPos[1], axisPos[2]);
      cylinder.length = tempCloud->bounding_box().diagonal_length();
      cylinder.axisDir = easy3d::vec3(axisDir[0], axisDir[1], axisDir[2]);
      cylinder.start = center - cylinder.axisDir * cylinder.length / 2.0f;
      cylinder.end = center + cylinder.axisDir * cylinder.length / 2.0f;
      cylinder.radius = radius;

      // add to result
      m_cylinders.push_back(cylinder);
    }
  }
}

}  // namespace ransac
