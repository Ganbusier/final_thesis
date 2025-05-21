#include "CustomCylinderConstructor.h"
#include <efficientRansac/Cylinder.h>

PrimitiveShape* CustomCylinderConstructor::Construct(
    const MiscLib::Vector<Vec3f>& points,
    const MiscLib::Vector<Vec3f>& normals) const 
{
    // create cylinder
    Cylinder cy;
    MiscLib::Vector<Vec3f> samples(points);
    std::copy(normals.begin(), normals.end(), std::back_inserter(samples));
    
    // try to initialize cylinder
    if (!cy.Init(samples)) {
        return nullptr;
    }
    
    // check if radius is in valid range
    if (cy.Radius() < min_radius_ || cy.Radius() > max_radius_) {
        return nullptr;
    }
    
    // create and return cylinder shape
    return new CylinderPrimitiveShape(cy);
} 