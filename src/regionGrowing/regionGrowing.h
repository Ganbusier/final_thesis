#ifndef REGION_GROWING_H
#define REGION_GROWING_H

#include <easy3d/algo/point_cloud_normals.h>
#include <easy3d/core/point_cloud.h>
#include <easy3d/fileio/point_cloud_io.h>

#include "cgalDefines_rg.h"

namespace regionGrowing {

struct Cylinder {
  Point_3 start;
  Point_3 end;
  FT radius;
  FT length;
  std::vector<size_t> inlierIndices;
};

struct CylinderRegionGrowingParams {
  std::size_t k = 16;
  FT maxDistance = FT(0.1);
  FT maxAngle = FT(25);
  FT minRadius = FT(0.01);
  FT maxRadius = FT(1.0);
  std::size_t minRegionSize = 4;
};

bool makePointSet(easy3d::PointCloud* pointCloud, Point_set& pointSet,
                  size_t knn = 16);

class CylinderRegionGrowing {
 private:
  CylinderRegionGrowingParams m_params;
  Point_set m_pointSet;
  Neighbor_query m_neighborQuery;
  Cylinder_Region_type m_regionType;
  Cylinder_Region_growing m_regionGrowing;
  std::vector<Primitive_and_region> m_regions;
  std::vector<Cylinder> m_cylinders;
  std::vector<size_t> m_unassignedIndices;
  std::vector<easy3d::vec3> m_unassignedPoints;

 private:
  void constructCylinders();
  void constructUnassignedIndices();
  void constructUnassignedPoints();

 public:
  CylinderRegionGrowing(const Point_set& pointSet,
                        const CylinderRegionGrowingParams& params)
      : m_params(params),
        m_pointSet(pointSet),
        m_neighborQuery(CGAL::Shape_detection::Point_set::make_k_neighbor_query(
            m_pointSet, CGAL::parameters::k_neighbors(m_params.k))),
        m_regionType(
            CGAL::Shape_detection::Point_set::
                make_least_squares_cylinder_fit_region(
                    m_pointSet,
                    CGAL::parameters::maximum_distance(m_params.maxDistance)
                        .maximum_angle(m_params.maxAngle)
                        .minimum_radius(m_params.minRadius)
                        .maximum_radius(m_params.maxRadius)
                        .minimum_region_size(m_params.minRegionSize))),
        m_regionGrowing(m_pointSet, m_neighborQuery, m_regionType) {}

  void detect();
  const std::vector<Primitive_and_region>& getRegions() const {
    return m_regions;
  }
  const std::vector<Cylinder>& getCylinders() const { return m_cylinders; }
  const std::vector<size_t>& getUnassignedIndices() const {
    return m_unassignedIndices;
  }
  const std::vector<easy3d::vec3>& getUnassignedPoints() const {
    return m_unassignedPoints;
  }
  void saveCylinderInfos(const std::string& filename);
  void saveUnassignedPoints(const std::string& filename);
  void save3DLineSegments(const std::string& filename);
};

}  // namespace regionGrowing

#endif  // REGION_GROWING_H