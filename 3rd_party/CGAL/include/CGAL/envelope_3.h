// Copyright (c) 2006  Tel-Aviv University (Israel).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
//
// $URL: https://github.com/CGAL/cgal/blob/v6.0.1/Envelope_3/include/CGAL/envelope_3.h $
// $Id: include/CGAL/envelope_3.h 50cfbde3b84 $
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-Commercial
//
//
// Author(s) : Ron Wein           <wein@post.tau.ac.il>
//             Baruch Zukerman    <baruchzu@post.tau.ac.il>
//             Efi Fogel          <efifogel@gmail.com>

#ifndef CGAL_ENVELOPE_3_H
#define CGAL_ENVELOPE_3_H

#include <CGAL/license/Envelope_3.h>


#include <CGAL/Envelope_3/Envelope_diagram_on_surface_2.h>
#include <CGAL/Envelope_3/Envelope_divide_and_conquer_3.h>
#include <CGAL/Arr_accessor.h>

namespace CGAL {

/*! Construct the lower envelope of a given set of surfaces.
 * \param begin An iterator for the first surface.
 * \param end A past-the-end iterator for the surfaces in the range.
 * \param min_diag Output: The minimization diagram.
 * \pre The value-type of InputIterator is Traits::Surface_3.
 */
template <typename InputIterator, typename GeomTraits, typename TopolTraits>
void lower_envelope_3(InputIterator begin, InputIterator end,
                      Envelope_diagram_on_surface_2<GeomTraits, TopolTraits>&
                        min_diagram) {
  using Gt = GeomTraits;
  using Tt = TopolTraits;

  using Edos_2 = Envelope_diagram_on_surface_2<Gt, Tt>;
  using Aos = typename Edos_2::Base_aos;
  using Envelope_algorithm = Envelope_divide_and_conquer_3<Gt, Aos>;
  Envelope_algorithm env_alg(min_diagram.geometry_traits(), ENVELOPE_LOWER);
  env_alg.construct_lu_envelope(begin, end, min_diagram);
}

/*! Construct the upper envelope of a given set of surfaces.
 * \param begin An iterator for the first surface.
 * \param end A past-the-end iterator for the surfaces in the range.
 * \param max_diag Output: The maximization diagram.
 * \pre The value-type of InputIterator is Traits::Surface_3.
 */
template <typename InputIterator, typename GeomTraits, typename TopolTraits>
void upper_envelope_3(InputIterator begin, InputIterator end,
                      Envelope_diagram_on_surface_2<GeomTraits, TopolTraits>&
                        max_diagram) {
  using Gt = GeomTraits;
  using Tt = TopolTraits;

  using Edos = Envelope_diagram_on_surface_2<Gt, Tt>;
  using Aos = typename Edos::Base_aos;
  using Envelope_algorithm = Envelope_divide_and_conquer_3<Gt, Aos>;
  Envelope_algorithm env_alg(max_diagram.geometry_traits(), ENVELOPE_UPPER);
  env_alg.construct_lu_envelope (begin, end, max_diagram);
}


/*! Construct the lower envelope of a given set of xy_monotone surfaces.
 * \param begin An iterator for the first xy-monotone surface.
 * \param end A past-the-end iterator for the xy_monotone surfaces in the range.
 * \param min_diag Output: The minimization diagram.
 * \pre The value-type of InputIterator is GeomTraits::Surface_3.
 */
template <typename InputIterator, typename GeomTraits, typename TopolTraits>
void lower_envelope_xy_monotone_3(InputIterator begin, InputIterator end,
                                  Envelope_diagram_on_surface_2
                                    <GeomTraits, TopolTraits>& min_diagram) {
  using Gt = GeomTraits;
  using Tt = TopolTraits;

  using Edos = Envelope_diagram_on_surface_2<Gt, Tt>;
  using Aos = typename Edos::Base_aos;
  using Envelope_algorithm = Envelope_divide_and_conquer_3<Gt, Aos>;
  Envelope_algorithm env_alg(min_diagram.geometry_traits(), ENVELOPE_LOWER);
  env_alg.construct_envelope_xy_monotone (begin, end, min_diagram);
}


/*! Construct the upper envelope of a given set of xy_monotone surfaces.
 * \param begin An iterator for the first xy_monotone surface.
 * \param end A past-the-end iterator for the xy_monotone surfaces in the range.
 * \param max_diag Output: The maximization diagram.
 * \pre The value-type of InputIterator is Traits::Surface_3.
 */
template <typename InputIterator, typename GeomTraits, typename TopolTraits>
void upper_envelope_xy_monotone_3(InputIterator begin, InputIterator end,
                                  Envelope_diagram_on_surface_2
                                    <GeomTraits, TopolTraits>& max_diagram) {
  using Gt = GeomTraits;
  using Tt = TopolTraits;

  using Edos = Envelope_diagram_on_surface_2<Gt, Tt>;
  using Aos = typename Edos::Base_aos;
  using Envelope_algorithm = Envelope_divide_and_conquer_3<Gt, Aos>;
  Envelope_algorithm env_alg(max_diagram.geometry_traits(), ENVELOPE_UPPER);
  env_alg.construct_envelope_xy_monotone (begin, end, max_diagram);
}


} //namespace CGAL

#endif
