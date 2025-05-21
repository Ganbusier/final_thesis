#ifndef RANSAC3D_H
#define RANSAC3D_H

#include <easy3d/3rd_party/ransac/RansacShapeDetector.h>
#include <easy3d/core/point_cloud.h>

#include <vector>

#include "CustomCylinderConstructor.h"


namespace ransac {

struct CustomCylinder {
    easy3d::vec3 start;
    easy3d::vec3 end;
    easy3d::vec3 axisDir;
    float radius;
    float length;
    std::vector<int> inlierIndices;
};

class CustomCylinderRANSAC {
 public:
  CustomCylinderRANSAC();
  ~CustomCylinderRANSAC();

  void setEpsilon(float epsilon) { m_epsilon = epsilon; }
  void setBitmapResolution(float resolution) { m_bitmapResolution = resolution; }
  void setNormalThreshold(float threshold) { m_normalThreshold = threshold; }
  void setMinSupport(int min_support) { m_minSupport = min_support; }
  void setProbability(float probability) { m_probability = probability; }

  void setRadiusRange(float min_radius, float max_radius) {
    m_cylinderConstructor.setRadiusRange(min_radius, max_radius);
  }

  void detect(easy3d::PointCloud* cloud);

 private:
  float m_epsilon = 0.01f;
  float m_bitmapResolution = 0.02f;
  float m_normalThreshold = 0.8f;
  int m_minSupport = 100;
  float m_probability = 0.01f;
  CustomCylinderConstructor m_cylinderConstructor;
  std::vector<CustomCylinder> m_cylinders;

};

}  // namespace ransac

#endif  // RANSAC3D_H
