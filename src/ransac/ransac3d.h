#ifndef RANSAC3D_H
#define RANSAC3D_H

#include <easy3d/core/point_cloud.h>

#include "cgalDefines_ransac.h"

namespace ransac {
struct CylinderInfo {
  easy3d::vec3 start;
  easy3d::vec3 end;
  float radius;
  float length;
  std::vector<int> inlierIndices;
};

struct Params {
  FT normalThreshold = 0.9;
  FT probability = 0.01;
  FT minPoints = 20;
  FT epsilon = 0.05;
  FT clusterEpsilon = 1.0;
};

class Ransac3d {
 public:
  Ransac3d(easy3d::PointCloud* pointCloud, const Params& params);

  void detect();

  const std::vector<Cylinder>& getCylinders() const { return m_cylinders; }

  const std::vector<CylinderInfo>& getCylinderInfos() const {
    return m_cylinderInfos;
  }

  const std::vector<int>& getLeftoverIndices() const {
    return m_leftoverIndices;
  }

  const std::vector<easy3d::vec3>& getLeftoverPoints() const {
    return m_leftoverPoints;
  }

 private:
  easy3d::PointCloud* m_pointCloud;
  Pwn_vector m_pwnVector;
  Efficient_ransac m_ransac;
  Efficient_ransac::Parameters m_params;
  std::vector<Cylinder> m_cylinders;
  std::vector<CylinderInfo> m_cylinderInfos;
  std::vector<int> m_leftoverIndices;
  std::vector<easy3d::vec3> m_leftoverPoints;

  void pointCloudToPwnVector();
  void storeCylinders();
  void storeCylinderInfos();
  void storeLeftoverIndices();
  void storeLeftoverPoints();
};

}  // namespace ransac

#endif  // RANSAC3D_H
