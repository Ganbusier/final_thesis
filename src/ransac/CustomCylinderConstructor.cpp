#include "CustomCylinderConstructor.h"
#include <easy3d/3rd_party/ransac/Cylinder.h>

PrimitiveShape* CustomCylinderConstructor::Construct(
    const MiscLib::Vector<Vec3f>& points,
    const MiscLib::Vector<Vec3f>& normals) const 
{
    // 创建圆柱体
    Cylinder cy;
    MiscLib::Vector<Vec3f> samples(points);
    std::copy(normals.begin(), normals.end(), std::back_inserter(samples));
    
    // 尝试初始化圆柱体
    if (!cy.Init(samples)) {
        return nullptr;
    }
    
    // 检查半径是否在有效范围内
    if (cy.Radius() < min_radius_ || cy.Radius() > max_radius_) {
        return nullptr;
    }
    
    // 创建并返回圆柱体形状
    return new CylinderPrimitiveShape(cy);
} 