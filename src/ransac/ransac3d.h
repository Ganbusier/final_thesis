#ifndef RANSAC3D_H
#define RANSAC3D_H

#include <easy3d/3rd_party/ransac/RansacShapeDetector.h>
#include <easy3d/core/point_cloud.h>

#include <vector>

#include "CustomCylinderConstructor.h"


namespace ransac {

struct Cylinder {
    easy3d::vec3 start;
    easy3d::vec3 end;
    float radius;
    std::vector<int> inlierIndices;
}

class CustomCylinderRANSAC {
 public:
  CustomCylinderRANSAC();
  ~CustomCylinderRANSAC();

  void setEpsilon(float epsilon) { epsilon_ = epsilon; }
  void setBitmapResolution(float resolution) {
    bitmap_resolution_ = resolution;
  }
  void setNormalThreshold(float threshold) { normal_threshold_ = threshold; }
  void setMinSupport(int min_support) { min_support_ = min_support; }
  void setProbability(float probability) { probability_ = probability; }

  void setRadiusRange(float min_radius, float max_radius) {
    cylinder_constructor_.setRadiusRange(min_radius, max_radius);
  }

  void detect(easy3d::PointCloud* cloud);

 private:
  float epsilon_ = 0.01f;
  float bitmap_resolution_ = 0.02f;
  float normal_threshold_ = 0.8f;
  int min_support_ = 100;
  float probability_ = 0.01f;
  CustomCylinderConstructor cylinder_constructor_;
  std::vector<CylinderPrimitiveShape> detected_cylinders_;
};

}  // namespace ransac

#endif  // RANSAC3D_H
