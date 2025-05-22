#ifndef CGAL_DEFINES_RANSAC_H
#define CGAL_DEFINES_RANSAC_H

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Point_set_3.h>
#include <CGAL/Point_with_normal_3.h>
#include <CGAL/Shape_detection/Efficient_RANSAC.h>
#include "customCylinder.h"

namespace ransac {
using Kernel = CGAL::Exact_predicates_inexact_constructions_kernel;
using FT = Kernel::FT;
using Point_with_normal = std::pair<Kernel::Point_3, Kernel::Vector_3>;
using Pwn_vector = std::vector<Point_with_normal>;
using Point_map = CGAL::First_of_pair_property_map<Point_with_normal>;
using Normal_map = CGAL::Second_of_pair_property_map<Point_with_normal>;

using Traits =
    CGAL::Shape_detection::Efficient_RANSAC_traits<Kernel, Pwn_vector, Point_map, Normal_map>;
using Efficient_ransac = CGAL::Shape_detection::Efficient_RANSAC<Traits>;
using Cylinder = CustomCylinder<Traits>;
} // namespace ransac

#endif // CGAL_DEFINES_RANSAC_H

