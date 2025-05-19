#ifndef REGION_GROWING_H
#define REGION_GROWING_H

#include "cgalDefines_rg.h"

namespace regionGrowing {

struct CylinderRegionGrowingParams {
  std::size_t k = 16;
  FT max_distance = FT(0.1);
  FT max_angle = FT(25);
  FT min_radius = FT(0.01);
  FT max_radius = FT(1.0);
  std::size_t min_region_size = 4;
}

class CylinderRegionGrowing {
}
}  // namespace regionGrowing

#endif  // REGION_GROWING_H