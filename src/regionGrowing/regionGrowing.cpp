#include <easy3d/core/graph.h>
#include <easy3d/fileio/graph_io.h>

#include "regionGrowing.h"


namespace regionGrowing {

bool makePointSet(easy3d::PointCloud* pointCloud, Point_set& pointSet,
                  size_t knn) {
  auto normals = pointCloud->get_vertex_property<easy3d::vec3>("v:normal");
  auto points = pointCloud->get_vertex_property<easy3d::vec3>("v:point");
  if (!normals) {
    LOG(INFO) << "Point cloud does not have normals, estimating normals";
    if (!easy3d::PointCloudNormals::estimate(pointCloud, knn)) {
      LOG(ERROR) << "Failed to estimate normals";
      return false;
    }
    normals = pointCloud->get_vertex_property<easy3d::vec3>("v:normal");
  }

  for (const auto& vertex : pointCloud->vertices()) {
    const easy3d::vec3 p = points[vertex];
    const easy3d::vec3 n = normals[vertex];
    pointSet.insert(Point_3(p[0], p[1], p[2]), Vector_3(n[0], n[1], n[2]));
  }
  LOG(INFO) << "Point set size: " << pointSet.size();
  return true;
}

void CylinderRegionGrowing::detect() {
  CGAL::Random random;
  m_regionGrowing.detect(std::back_inserter(m_regions));
  if (m_regions.empty()) {
    LOG(INFO) << "No regions found, all points are assigned";
    constructUnassignedIndices();
    constructUnassignedPoints();
    return;
  }
  constructCylinders();
  constructUnassignedIndices();
  constructUnassignedPoints();
}

void CylinderRegionGrowing::constructCylinders() {
  if (m_regions.empty()) {
    LOG(INFO) << "No regions found, cannot construct cylinders";
    return;
  }

  for (const auto& region : m_regions) {
    const auto& inlierIndices = region.second;
    const auto& cylinderPrimitive = region.first;
    Cylinder newCylinder;
    newCylinder.radius = cylinderPrimitive.radius;
    newCylinder.inlierIndices.reserve(inlierIndices.size());
    for (const auto& index : inlierIndices) {
      newCylinder.inlierIndices.push_back(index);
    }

    // get the xyz minmax of the inlier points
    CGAL::Bbox_3 bbox;
    for (const auto& index : inlierIndices) {
      bbox = bbox + m_pointSet.point(index).bbox();
    }

    // calculate the length of the cylinder using the bbox
    auto bbox_radius =
        0.5f * std::sqrt(CGAL::squared_distance(
                   Point_3(bbox.xmin(), bbox.ymin(), bbox.zmin()),
                   Point_3(bbox.xmax(), bbox.ymax(), bbox.zmax())));

    // calculate the start and end points of the cylinder
    auto center = cylinderPrimitive.axis.point(0);
    auto direction = cylinderPrimitive.axis.to_vector();
    newCylinder.start = center + direction * bbox_radius;
    newCylinder.end = center - direction * bbox_radius;
    newCylinder.length =
        std::sqrt(CGAL::squared_distance(newCylinder.start, newCylinder.end));
    m_cylinders.push_back(newCylinder);
  }
}

void CylinderRegionGrowing::constructUnassignedIndices() {
  if (m_regions.empty()) {
    LOG(INFO) << "No regions found, all points are assigned";
    for (size_t i = 0; i < m_pointSet.size(); ++i) {
      m_unassignedIndices.push_back(i);
    }
    return;
  }

  size_t num_unassigned_points = m_pointSet.size();
  for (const auto& region : m_regions) {
    const auto& inlierIndices = region.second;
    num_unassigned_points -= inlierIndices.size();
  }
  m_unassignedIndices.reserve(num_unassigned_points);
  std::vector<bool> assigned(m_pointSet.size(), false);
  for (const auto& region : m_regions) {
    const auto& inlierIndices = region.second;
    for (const auto& index : inlierIndices) {
      assigned[index] = true;
    }
  }
  for (size_t i = 0; i < m_pointSet.size(); ++i) {
    if (!assigned[i]) {
      m_unassignedIndices.push_back(i);
    }
  }
}

void CylinderRegionGrowing::constructUnassignedPoints() {
  m_unassignedPoints.clear();
  for (const auto& index : m_unassignedIndices) {
    auto p = m_pointSet.point(index);
    m_unassignedPoints.push_back(easy3d::vec3(p.x(), p.y(), p.z()));
  }
}

void CylinderRegionGrowing::saveCylinderInfos(const std::string& filename) {
  // save the cylinder infos to a csv file
  std::ofstream file(filename);
  file << "radius,length,start_x,start_y,start_z,end_x,end_y,end_z\n";
  for (const auto& cylinder : m_cylinders) {
    file << cylinder.radius << "," << cylinder.length << ","
         << cylinder.start.x() << "," << cylinder.start.y() << ","
         << cylinder.start.z() << "," << cylinder.end.x() << ","
         << cylinder.end.y() << "," << cylinder.end.z() << "\n";
  }
  file.close();
  LOG(INFO) << "Cylinder infos saved to " << filename;
}

void CylinderRegionGrowing::saveUnassignedPoints(const std::string& filename) {
  // save the unassigned points to a ply file
  easy3d::PointCloud* unassignedCloud = new easy3d::PointCloud;
  for (const auto& point : m_unassignedPoints) {
    unassignedCloud->add_vertex(point);
  }
  easy3d::io::save_ply(filename, unassignedCloud, false);
  LOG(INFO) << "Unassigned points saved to " << filename;
}

void CylinderRegionGrowing::save3DLineSegments(const std::string& filename) {
  easy3d::Graph* graph = new easy3d::Graph;
  for (const auto& cylinder : m_cylinders) {
    easy3d::vec3 start = easy3d::vec3(cylinder.start.x(), cylinder.start.y(),
                                       cylinder.start.z());
    easy3d::vec3 end = easy3d::vec3(cylinder.end.x(), cylinder.end.y(),
                                     cylinder.end.z());
    easy3d::Graph::Vertex v1 = graph->add_vertex(start);
    easy3d::Graph::Vertex v2 = graph->add_vertex(end);
    graph->add_edge(v1, v2);
  }
  easy3d::io::save_ply(filename, graph, false);
  LOG(INFO) << "3D line segments saved to " << filename;
}

}  // namespace regionGrowing
