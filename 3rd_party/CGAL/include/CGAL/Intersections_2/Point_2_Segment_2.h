// Copyright (c) 2000
// Utrecht University (The Netherlands),
// ETH Zurich (Switzerland),
// INRIA Sophia-Antipolis (France),
// Max-Planck-Institute Saarbruecken (Germany),
// and Tel-Aviv University (Israel).  All rights reserved.
//
// This file is part of CGAL (www.cgal.org)
//
// $URL: https://github.com/CGAL/cgal/blob/v6.0.1/Intersections_2/include/CGAL/Intersections_2/Point_2_Segment_2.h $
// $Id: include/CGAL/Intersections_2/Point_2_Segment_2.h 50cfbde3b84 $
// SPDX-License-Identifier: LGPL-3.0-or-later OR LicenseRef-Commercial
//
//
// Author(s)     : Geert-Jan Giezeman


#ifndef CGAL_INTERSECTIONS_2_POINT_2_SEGMENT_2_H
#define CGAL_INTERSECTIONS_2_POINT_2_SEGMENT_2_H

#include <CGAL/Segment_2.h>
#include <CGAL/Point_2.h>
#include <CGAL/Intersection_traits_2.h>

namespace CGAL {

namespace Intersections {

namespace internal {

template <class K>
inline
typename K::Boolean
do_intersect(const typename K::Point_2 &pt,
             const typename K::Segment_2 &seg,
             const K&)
{
    return seg.has_on(pt);
}

template <class K>
inline
typename K::Boolean
do_intersect(const typename K::Segment_2 &seg,
             const typename K::Point_2 &pt,
             const K&)
{
    return seg.has_on(pt);
}


template <class K>
inline
typename CGAL::Intersection_traits
<K, typename K::Point_2, typename K::Segment_2>::result_type
intersection(const typename K::Point_2 &pt,
             const typename K::Segment_2 &seg,
             const K& k)
{
  if (do_intersect(pt,seg, k)) {
    return intersection_return<typename K::Intersect_2, typename K::Point_2, typename K::Segment_2>(pt);
  }
  return intersection_return<typename K::Intersect_2, typename K::Point_2, typename K::Segment_2>();
}

template <class K>
inline
typename CGAL::Intersection_traits
<K, typename K::Segment_2, typename K::Point_2>::result_type
intersection( const typename K::Segment_2 &seg,
              const typename K::Point_2 &pt,
              const K& k)
{
  return internal::intersection(pt, seg, k);
}

} // namespace internal
} // namespace Intersections

CGAL_INTERSECTION_FUNCTION(Point_2, Segment_2, 2)
CGAL_DO_INTERSECT_FUNCTION(Point_2, Segment_2, 2)

} // namespace CGAL

#endif // CGAL_INTERSECTIONS_2_POINT_2_SEGMENT_2_H
