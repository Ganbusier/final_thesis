// Copyright (c) 2003,2004,2005,2006  INRIA Sophia-Antipolis (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
//
// $URL: https://github.com/CGAL/cgal/blob/v6.0.1/Segment_Delaunay_graph_2/include/CGAL/Segment_Delaunay_graph_2/Filtered_traits_concept_check_tags.h $
// $Id: include/CGAL/Segment_Delaunay_graph_2/Filtered_traits_concept_check_tags.h 50cfbde3b84 $
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-Commercial
//
//
// Author(s)     : Menelaos Karavelas <mkaravel@iacm.forth.gr>



#ifndef CGAL_SEGMENT_DELAUNAY_GRAPH_2_FILTERED_TRAITS_CONCEPT_CHECK_TAGS_H
#define CGAL_SEGMENT_DELAUNAY_GRAPH_2_FILTERED_TRAITS_CONCEPT_CHECK_TAGS_H

#include <CGAL/license/Segment_Delaunay_graph_2.h>


#include <CGAL/Segment_Delaunay_graph_2/basic.h>

namespace CGAL {

namespace SegmentDelaunayGraph_2 {

namespace Internal {

  template<class D, class T, int>
  struct Concept_check_tags {};

  template<class D>
  struct Concept_check_tags<D,Integral_domain_without_division_tag,2>
  {
    Concept_check_tags() {
      THE_2ND_TEMPLATE_PARAMETER_MUST_EITHER_BE_Field_tag_OR_Field_with_sqrt_tag
      ( D() );
    }
  };

  template<class D>
  struct Concept_check_tags<D,Integral_domain_without_division_tag,4> {
    Concept_check_tags() {
      THE_4TH_TEMPLATE_PARAMETER_MUST_EITHER_BE_Field_tag_OR_Field_with_sqrt_tag
      ( D() );
    }
  };

  template<class D>
  struct Concept_check_tags<D,Integral_domain_without_division_tag,6> {
    Concept_check_tags() {
      THE_6TH_TEMPLATE_PARAMETER_MUST_EITHER_BE_Field_tag_OR_Field_with_sqrt_tag
      ( D() );
    }
  };

  //-------------------------------------------------------------------------

  template<class D>
  struct Concept_check_tags<D,Euclidean_ring_tag,2>
  {
    Concept_check_tags() {
      THE_2ND_TEMPLATE_PARAMETER_MUST_EITHER_BE_Field_tag_OR_Field_with_sqrt_tag
      ( D() );
    }
  };

  template<class D>
  struct Concept_check_tags<D,Euclidean_ring_tag,4> {
    Concept_check_tags() {
      THE_4TH_TEMPLATE_PARAMETER_MUST_EITHER_BE_Field_tag_OR_Field_with_sqrt_tag
      ( D() );
    }
  };

  template<class D>
  struct Concept_check_tags<D,Euclidean_ring_tag,6> {
    Concept_check_tags() {
      THE_6TH_TEMPLATE_PARAMETER_MUST_EITHER_BE_Field_tag_OR_Field_with_sqrt_tag
      ( D() );
    }
  };

  //=========================================================================

  template<class D, class T, int>
  struct Concept_check_tags_wi {};

  template<class D>
  struct Concept_check_tags_wi<D,Field_tag,2>
  {
    Concept_check_tags_wi() {
      THE_2ND_TEMPLATE_PARAMETER_MUST_EITHER_BE_Integral_domain_without_division_tag_OR_Field_with_sqrt_tag
      ( D() );
    }
  };

  template<class D>
  struct Concept_check_tags_wi<D,Field_tag,4> {
    Concept_check_tags_wi() {
      THE_4TH_TEMPLATE_PARAMETER_MUST_EITHER_BE_Integral_domain_without_division_tag_OR_Field_with_sqrt_tag
      ( D() );
    }
  };

  template<class D>
  struct Concept_check_tags_wi<D,Field_tag,6> {
    Concept_check_tags_wi() {
      THE_6TH_TEMPLATE_PARAMETER_MUST_EITHER_BE_Integral_domain_without_division_tag_OR_Field_with_sqrt_tag
      ( D() );
    }
  };

  //-------------------------------------------------------------------------

  template<class D>
  struct Concept_check_tags_wi<D,Euclidean_ring_tag,2>
  {
    Concept_check_tags_wi() {
      THE_2ND_TEMPLATE_PARAMETER_MUST_EITHER_BE_Integral_domain_without_division_tag_OR_Field_with_sqrt_tag
      ( D() );
    }
  };

  template<class D>
  struct Concept_check_tags_wi<D,Euclidean_ring_tag,4> {
    Concept_check_tags_wi() {
      THE_4TH_TEMPLATE_PARAMETER_MUST_EITHER_BE_Integral_domain_without_division_tag_OR_Field_with_sqrt_tag
      ( D() );
    }
  };

  template<class D>
  struct Concept_check_tags_wi<D,Euclidean_ring_tag,6> {
    Concept_check_tags_wi() {
      THE_6TH_TEMPLATE_PARAMETER_MUST_EITHER_BE_Integral_domain_without_division_tag_OR_Field_with_sqrt_tag
      ( D() );
    }
  };

} // namespace Internal

} //namespace SegmentDelaunayGraph_2

} //namespace CGAL

#endif // CGAL_SEGMENT_DELAUNAY_GRAPH_2_FILTERED_TRAITS_CONCEPT_CHECK_TAGS_H
