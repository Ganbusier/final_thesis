#ifndef CGAL_DEFINITIONS_H
#define CGAL_DEFINITIONS_H

#include <CGAL/Point_set_3.h>
#include <CGAL/Simple_cartesian.h>

using Kernel = CGAL::Simple_cartesian<double>;
using Point_3 = typename Kernel::Point_3;
using Vector_3 = typename Kernel::Vector_3;
using FT = typename Kernel::FT;

using Point_set = CGAL::Point_set_3<Point_3>;
using Point_map = typename Point_set::Point_map;
using Normal_map = typename Point_set::Vector_map;


#endif // CGAL_DEFINITIONS_H

