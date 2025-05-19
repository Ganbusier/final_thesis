#ifndef CGAL_DEFINES_RG_H
#define CGAL_DEFINES_RG_H

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Point_set_3.h>
#include <CGAL/Shape_detection/Region_growing/Point_set.h>
#include <CGAL/Shape_detection/Region_growing/Region_growing.h>


using Kernel = CGAL::Simple_cartesian<double>;
using Point_3 = Kernel::Point_3;
using Vector_3 = Kernel::Vector_3;

using Point_set = CGAL::Point_set_3<Point_3>;
using Point_map = typename Point_set::Point_map;
using Normal_map = typename Point_set::Vector_map;

using Neighbor_query =
    CGAL::Shape_detection::Point_set::K_neighbor_query_for_point_set<Point_set>;
using Cylinder_Region_type = CGAL::Shape_detection::Point_set::
    Least_squares_cylinder_fit_region_for_point_set<Point_set>;
using Cylinder_Region_growing =
    CGAL::Shape_detection::Region_growing<Neighbor_query, Cylinder_Region_type>;

#endif  // CGAL_DEFINES_RG_H