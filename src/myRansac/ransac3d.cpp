#include <easy3d/algo/point_cloud_normals.h>
#include <efficientRansac/PointCloud.h>

#include "ransac3d.h"


namespace ransac {

CustomCylinderRANSAC::CustomCylinderRANSAC() {}

CustomCylinderRANSAC::~CustomCylinderRANSAC() {}

std::vector<CustomCylinder> CustomCylinderRANSAC::getCylinders() const {
  return m_cylinders;
}

std::vector<int> CustomCylinderRANSAC::getLeftOverIndices() const {
  return m_leftOverIndices;
}

void CustomCylinderRANSAC::detect(easy3d::PointCloud* cloud) {
  LOG(INFO) << "Start Ransac3d";
  m_cylinders.clear();

  // estimate normals
  auto points = cloud->get_vertex_property<easy3d::vec3>("v:point");
  auto normals = cloud->get_vertex_property<easy3d::vec3>("v:normal");
  if (!normals) {
    LOG(INFO) << "Point cloud does not have normals, estimating normals with "
                 "k-neighbors = 16";
    if (!easy3d::PointCloudNormals::estimate(cloud, 16)) {
      LOG(ERROR) << "Failed to estimate normals";
      return;
    }
    normals = cloud->get_vertex_property<easy3d::vec3>("v:normal");
  }

  // convert point cloud format
  LOG(INFO) << "Converting point cloud format";
  PointCloud pc;
  pc.resize(cloud->n_vertices());
  for (size_t i = 0; i < cloud->n_vertices(); ++i) {
    easy3d::PointCloud::Vertex v(i);
    const auto& p = points[v];
    const auto& n = normals[v];
    pc[i] = Point(Vec3f(p.x, p.y, p.z), Vec3f(n.x, n.y, n.z));
    pc[i].index = i;
  }

  const easy3d::Box3& box = cloud->bounding_box();
  pc.setBBox(Vec3f(static_cast<float>(box.min_coord(0)),
                   static_cast<float>(box.min_coord(1)),
                   static_cast<float>(box.min_coord(2))),
             Vec3f(static_cast<float>(box.max_coord(0)),
                   static_cast<float>(box.max_coord(1)),
                   static_cast<float>(box.max_coord(2))));

  // set ransac options
  LOG(INFO) << "Setting ransac options";
  RansacShapeDetector::Options ransacOptions;
  ransacOptions.m_epsilon = m_epsilon * pc.getScale();
  ransacOptions.m_bitmapEpsilon = m_bitmapResolution * pc.getScale();
  ransacOptions.m_normalThresh = m_normalThreshold;
  ransacOptions.m_minSupport = m_minSupport;
  ransacOptions.m_probability = m_probability;

  RansacShapeDetector detector(ransacOptions);

  // add custom cylinder constructor
  detector.Add(&m_cylinderConstructor);

  // detect
  LOG(INFO) << "Detecting cylinders";
  MiscLib::Vector<std::pair<MiscLib::RefCountPtr<PrimitiveShape>, size_t>>
      shapes;
  detector.Detect(pc, 0, pc.size(), &shapes);

  if (shapes.size() == 0) {
    LOG(INFO) << "No cylinders found";
    return;
  }

  PointCloud::reverse_iterator start = pc.rbegin();
  MiscLib::Vector<std::pair<MiscLib::RefCountPtr<PrimitiveShape>,
                            size_t>>::const_iterator shape_itr = shapes.begin();

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
      easy3d::vec3 center = box.center();
      cylinder.length = tempCloud->bounding_box().diagonal_length();
      cylinder.axisDir = easy3d::vec3(axisDir[0], axisDir[1], axisDir[2]);
      cylinder.start = center - cylinder.axisDir * cylinder.length / 2.0f;
      cylinder.end = center + cylinder.axisDir * cylinder.length / 2.0f;
      cylinder.radius = radius;

      // add to result
      m_cylinders.push_back(cylinder);
    }
  }

  // get left over indices
  m_leftOverIndices.clear();
  for (size_t i = 0; i < cloud->n_vertices(); ++i) {
    m_leftOverIndices.push_back(i);
  }
  for (auto cylinder : m_cylinders) {
    for (auto index : cylinder.inlierIndices) {
      m_leftOverIndices.erase(std::remove(m_leftOverIndices.begin(),
                                          m_leftOverIndices.end(), index),
                              m_leftOverIndices.end());
    }
  }

  LOG(INFO) << "Detected " << m_cylinders.size() << " cylinders";
  LOG(INFO) << "Left over indices size: " << m_leftOverIndices.size();
}

}  // namespace ransac
