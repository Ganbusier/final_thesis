// Copyright (c) 2006,2007,2009,2010,2011 Tel-Aviv University (Israel).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
//
// $URL: https://github.com/CGAL/cgal/blob/v6.0.1/Arrangement_on_surface_2/include/CGAL/Arr_point_location/Arr_naive_point_location_impl.h $
// $Id: include/CGAL/Arr_point_location/Arr_naive_point_location_impl.h 50cfbde3b84 $
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-Commercial
//
//
// Author(s)     : Ron Wein   <wein@post.tau.ac.il>
//                 (based on old version by Eyal Flato)
//                 Efi Fogel  <efif@post.tau.ac.il>

#ifndef CGAL_ARR_NAIVE_POINT_LOCATION_FUNCTIONS_H
#define CGAL_ARR_NAIVE_POINT_LOCATION_FUNCTIONS_H

#include <CGAL/license/Arrangement_on_surface_2.h>


/*! \file
 * Member-function definitions for the Arr_naive_point_location<Arrangement>
 * class.
 */

namespace CGAL {

//-----------------------------------------------------------------------------
// Locate the arrangement feature containing the given point.
//
template <typename Arrangement>
typename Arr_naive_point_location<Arrangement>::Result_type
Arr_naive_point_location<Arrangement>::locate(const Point_2& p) const
{
  // Go over the arrangement vertices and check whether one of them equals
  // the query point.
  auto equal = geom_traits->equal_2_object();

  for (auto vit = p_arr->vertices_begin(); vit != p_arr->vertices_end(); ++vit) {
    Vertex_const_handle vh = vit;
    if (equal(p, vh->point())) return make_result(vh);
  }

  // Go over arrangement halfedges and check whether one of them contains
  // the query point in its interior.
  auto is_in_x_range = geom_traits->is_in_x_range_2_object();
  auto compare_y_at_x = geom_traits->compare_y_at_x_2_object();

  for (auto eit = p_arr->edges_begin(); eit != p_arr->edges_end(); ++eit) {
    Halfedge_const_handle hh = eit;
    if (is_in_x_range(hh->curve(), p) && compare_y_at_x(p, hh->curve()) == EQUAL)
      return make_result(hh);
  }

  // Go over all faces and locate the innermost one that contains the query
  // point in its interior.
  Face_const_handle f_inner;
  const Face_const_handle invalid_f;

  for (auto fit = p_arr->faces_begin(); fit != p_arr->faces_end(); ++fit) {
    Face_const_handle fh = fit;
    if (top_traits->is_in_face(&(*fh), p, nullptr)) {
      // The current face contains p in its interior.
      if (f_inner == invalid_f ||
          f_inner->is_unbounded() ||
          f_inner->number_of_outer_ccbs() == 0)
      {
        // This is the first face that contains p we encounter:
        f_inner = fh;
      }
      else if (! fh->is_unbounded() && fh->number_of_outer_ccbs() > 0) {
        // As we have already some other containing face, one face must be
        // contained inside the other. To check that, we select a
        // representative vertex of inner_f and check whether it is contained
        // in our current face.

        // This is a workaround for MSVC. For some reason the compiler barfs
        // when the iterator is not saved in a variable and only then the
        // source() of its value_type is accessed.
        typename Arrangement_2::Outer_ccb_const_iterator it =
          fh->outer_ccbs_begin();
        Vertex_const_handle v = (*it)->source();
        if (top_traits->is_in_face(&(*f_inner), v->point(), nullptr))
          f_inner = fh;
      }
    }
  }

  // Return the innermost face.
  CGAL_assertion(f_inner != invalid_f);
  return make_result(f_inner);
}

} //namespace CGAL

#endif
