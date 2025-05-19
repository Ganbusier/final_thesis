#include "regionGrowing.h"

namespace regionGrowing {

void CylinderRegionGrowing::detect() {
  CGAL::Random random;
  m_regionGrowing.detect(std::back_inserter(m_regions));
}

const std::vector<Primitive_and_region>& CylinderRegionGrowing::getRegions() const {
  return m_regions;
}

}  // namespace regionGrowing
