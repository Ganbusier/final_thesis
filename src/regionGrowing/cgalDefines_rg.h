#ifndef CGAL_DEFINES_RG_H
#define CGAL_DEFINES_RG_H

#include <CGAL/Point_set_3.h>
#include <CGAL/Shape_detection/Region_growing/Point_set.h>
#include <CGAL/Shape_detection/Region_growing/Region_growing.h>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Random.h>

using Kernel = CGAL::Simple_cartesian<double>;
using Point_3 = typename Kernel::Point_3;
using Vector_3 = typename Kernel::Vector_3;
using FT = typename Kernel::FT;

using Point_set = CGAL::Point_set_3<Point_3>;
using Point_map = typename Point_set::Point_map;
using Normal_map = typename Point_set::Vector_map;

using Neighbor_query =
    CGAL::Shape_detection::Point_set::K_neighbor_query_for_point_set<Point_set>;
using Cylinder_Region_type = CGAL::Shape_detection::Point_set::
    Least_squares_cylinder_fit_region_for_point_set<Point_set>;
using Cylinder_Region_growing =
    CGAL::Shape_detection::Region_growing<Neighbor_query, Cylinder_Region_type>;

using Primitive_and_region = typename Cylinder_Region_growing::Primitive_and_region;

#endif  // CGAL_DEFINES_RG_H