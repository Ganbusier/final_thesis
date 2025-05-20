#pragma once

#include <easy3d/core/point_cloud.h>
#include <easy3d/3rd_party/ransac/RansacShapeDetector.h>
#include <vector>

#include "CustomCylinderConstructor.h"

class CustomCylinderRANSAC {
public:
    CustomCylinderRANSAC();
    ~CustomCylinderRANSAC();

    // 设置 RANSAC 参数
    void setEpsilon(float epsilon) { epsilon_ = epsilon; }
    void setBitmapResolution(float resolution) { bitmap_resolution_ = resolution; }
    void setNormalThreshold(float threshold) { normal_threshold_ = threshold; }
    void setMinSupport(int min_support) { min_support_ = min_support; }
    void setProbability(float probability) { probability_ = probability; }

    // 设置圆柱体半径范围
    void setRadiusRange(float min_radius, float max_radius) {
        cylinder_constructor_.setRadiusRange(min_radius, max_radius);
    }

    // 执行检测
    std::vector<CylinderPrimitiveShape> detect(easy3d::PointCloud* cloud);

private:
    float epsilon_ = 0.01f;
    float bitmap_resolution_ = 0.02f;
    float normal_threshold_ = 0.8f;
    int min_support_ = 100;
    float probability_ = 0.01f;
    CustomCylinderConstructor cylinder_constructor_;
}; 