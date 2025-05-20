#ifndef CUSTOM_CYLINDER_CONSTRUCTOR_H
#define CUSTOM_CYLINDER_CONSTRUCTOR_H

#include <easy3d/3rd_party/ransac/CylinderPrimitiveShapeConstructor.h>
#include <easy3d/3rd_party/ransac/CylinderPrimitiveShape.h>

class CustomCylinderConstructor : public CylinderPrimitiveShapeConstructor {
public:
    CustomCylinderConstructor(float min_radius = 0.0f, float max_radius = std::numeric_limits<float>::max())
        : min_radius_(min_radius), max_radius_(max_radius) {}

    // 重写 Construct 方法
    PrimitiveShape* Construct(const MiscLib::Vector<Vec3f>& points,
                            const MiscLib::Vector<Vec3f>& normals) const override;

    // 设置半径范围
    void setRadiusRange(float min_radius, float max_radius) {
        min_radius_ = min_radius;
        max_radius_ = max_radius;
    }

private:
    float min_radius_;
    float max_radius_;
};

#endif // CUSTOM_CYLINDER_CONSTRUCTOR_H 