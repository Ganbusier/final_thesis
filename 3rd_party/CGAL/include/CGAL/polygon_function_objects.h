// Copyright (c) 2000  Max-Planck-Institute Saarbruecken (Germany).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
//
// $URL: https://github.com/CGAL/cgal/blob/v6.0.1/Partition_2/include/CGAL/polygon_function_objects.h $
// $Id: include/CGAL/polygon_function_objects.h 50cfbde3b84 $
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-Commercial
//
//
// Author(s)     : Susan Hert <hert@mpi-sb.mpg.de>

#ifndef CGAL_POLYGON_FUNCTION_OBJECTS_H
#define CGAL_POLYGON_FUNCTION_OBJECTS_H

#include <CGAL/license/Partition_2.h>


#include <CGAL/is_y_monotone_2.h>

namespace CGAL {

template <class Traits>
class Is_vacuously_valid
{
  public:

     Is_vacuously_valid(Traits ) {}

     template <class ForwardIterator>
     bool operator()(ForwardIterator, ForwardIterator)
     {  return true; }

};


template <class Traits>
class Is_convex_2
{
  public:
     Is_convex_2(const Traits& t): traits(t) {}

     template <class ForwardIterator>
     bool operator()(ForwardIterator first, ForwardIterator last)
     {  return is_convex_2(first, last, traits); }

  private:
     const Traits& traits;
};

template <class Traits>
class Is_y_monotone_2
{
  public:
     Is_y_monotone_2(const Traits& t): traits(t) {}

     template <class ForwardIterator>
     bool operator()(ForwardIterator first, ForwardIterator last)
     {  return is_y_monotone_2(first, last, traits); }

  private:
     const Traits& traits;
};

}

#endif // CGAL_POLYGON_FUNCTION_OBJECTS_H
