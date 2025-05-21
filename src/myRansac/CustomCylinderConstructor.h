#ifndef CUSTOM_CYLINDER_CONSTRUCTOR_H
#define CUSTOM_CYLINDER_CONSTRUCTOR_H

#include <efficientRansac/CylinderPrimitiveShapeConstructor.h>
#include <efficientRansac/CylinderPrimitiveShape.h>

class CustomCylinderConstructor : public CylinderPrimitiveShapeConstructor {
public:
    CustomCylinderConstructor(float min_radius = 0.0001f, float max_radius = 1e6f)
        : min_radius_(min_radius), max_radius_(max_radius) {}

    // rewrite the Construct method
    PrimitiveShape* Construct(const MiscLib::Vector<Vec3f>& points,
                            const MiscLib::Vector<Vec3f>& normals) const override;

    // set radius range
    void setRadiusRange(float min_radius, float max_radius) {
        min_radius_ = min_radius;
        max_radius_ = max_radius;
    }

private:
    float min_radius_;
    float max_radius_;
};

#endif // CUSTOM_CYLINDER_CONSTRUCTOR_H 