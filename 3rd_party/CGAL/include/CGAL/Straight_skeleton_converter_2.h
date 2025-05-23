// Copyright (c) 2005-2008 Fernando Luis Cacciola Carballal. All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
//
// $URL: https://github.com/CGAL/cgal/blob/v6.0.1/Straight_skeleton_2/include/CGAL/Straight_skeleton_converter_2.h $
// $Id: include/CGAL/Straight_skeleton_converter_2.h 50cfbde3b84 $
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-Commercial
//
// Author(s)     : Fernando Cacciola <fernando_cacciola@ciudad.com.ar>

#ifndef CGAL_STRAIGHT_SKELETON_CONVERTER_2_H
#define CGAL_STRAIGHT_SKELETON_CONVERTER_2_H 1

#include <CGAL/license/Straight_skeleton_2.h>

#include <CGAL/Straight_skeleton_2/Straight_skeleton_aux.h>
#include <CGAL/Trisegment_2.h>

#include <CGAL/assertions.h>
#include <CGAL/Cartesian_converter.h>

#include <memory>


#include <vector>

namespace CGAL {

template<class Source_skeleton_
        ,class Target_skeleton_
        ,class NT_converter = typename internal::Default_converter<typename Source_skeleton_::Traits
                                                               ,typename Target_skeleton_::Traits
                                                               >::Type
        >
struct Straight_skeleton_items_converter_2: Cartesian_converter< typename Source_skeleton_::Traits
                                                               , typename Target_skeleton_::Traits
                                                               , NT_converter
                                                               >
{
  typedef Source_skeleton_ Source_skeleton ;
  typedef Target_skeleton_ Target_skeleton ;

  typedef typename Source_skeleton::Traits Source_traits ;
  typedef typename Target_skeleton::Traits Target_traits ;

  typedef CGAL_SS_i::Segment_2_with_ID<Source_traits> Source_segment_2_with_ID;
  typedef CGAL_SS_i::Segment_2_with_ID<Target_traits> Target_segment_2_with_ID;

  typedef typename Source_traits::FT Source_FT;
  typedef typename Source_traits::Segment_2 Source_segment_2;
  typedef typename Target_traits::Segment_2 Target_segment_2;
  typedef Trisegment_2<Source_traits, Source_segment_2> Source_trisegment_2;
  typedef Trisegment_2<Target_traits, Target_segment_2> Target_trisegment_2;
  typedef Trisegment_2_ptr<Source_trisegment_2> Source_trisegment_2_ptr;
  typedef Trisegment_2_ptr<Target_trisegment_2> Target_trisegment_2_ptr;

  // Same as above, but for Segment with IDs...
  typedef Trisegment_2<Source_traits, Source_segment_2_with_ID> Source_trisegment_2_with_ID;
  typedef Trisegment_2<Target_traits, Target_segment_2_with_ID> Target_trisegment_2_with_ID;
  typedef Trisegment_2_ptr<Source_trisegment_2_with_ID> Source_trisegment_2_with_ID_ptr;
  typedef Trisegment_2_ptr<Target_trisegment_2_with_ID> Target_trisegment_2_with_ID_ptr;

  typedef Cartesian_converter<Source_traits,Target_traits> Base ;

  typedef typename Source_skeleton::Vertex_const_handle   Source_vertex_const_handle ;
  typedef typename Source_skeleton::Halfedge_const_handle Source_halfedge_const_handle ;
  typedef typename Source_skeleton::Face_const_handle     Source_face_const_handle  ;

  typedef typename Target_skeleton::Vertex    Target_vertex ;
  typedef typename Target_skeleton::Halfedge  Target_halfedge ;
  typedef typename Target_skeleton::Face      Target_face ;

  using Base::operator();

  Target_vertex operator() ( Source_vertex_const_handle aV ) const
  {
    CGAL_assertion( handle_assigned(aV) ) ;

    return Target_vertex( aV->id()
                        , this->operator()(aV->point())
                        , this->operator()(aV->time ())
                        , aV->is_split()
                        , aV->has_infinite_time()
                        ) ;
  }

  Target_halfedge operator() ( Source_halfedge_const_handle aH ) const
  {
    CGAL_assertion( handle_assigned(aH) ) ;

    return Target_halfedge( aH->id(), aH->slope() ) ;
  }

  Target_face operator() ( Source_face_const_handle aF ) const
  {
    CGAL_assertion( handle_assigned(aF) ) ;

    return Target_face( aF->id() );
  }

  Target_segment_2_with_ID operator() ( const Source_segment_2_with_ID& aS ) const
  {
    return Target_segment_2_with_ID(this->operator()(
      static_cast<const typename Source_segment_2_with_ID::Base&>(aS)), aS.mID);
  }

  Target_trisegment_2_ptr operator() ( const Source_trisegment_2_ptr& aT ) const
  {
    const auto& lSe0 = aT->e0();
    const auto& lSe1 = aT->e1();
    const auto& lSe2 = aT->e2();

    const Source_FT& lW0 = aT->w0();
    const Source_FT& lW1 = aT->w1();
    const Source_FT& lW2 = aT->w2();

    Trisegment_collinearity lCollinearity = aT->collinearity();
    std::size_t lId = aT->id();

    Target_trisegment_2_ptr rT = Target_trisegment_2_ptr(
                                   new Target_trisegment_2(this->operator()(lSe0),
                                                           this->operator()(lW0),
                                                           this->operator()(lSe1),
                                                           this->operator()(lW1),
                                                           this->operator()(lSe2),
                                                           this->operator()(lW2),
                                                           lCollinearity, lId));

    if ( aT->child_l() )
      rT->set_child_l(this->operator()(aT->child_l()));
    if ( aT->child_r() )
      rT->set_child_r(this->operator()(aT->child_r()));
    if ( aT->child_t() )
      rT->set_child_t(this->operator()(aT->child_t()));

    return rT;
  }

  Target_trisegment_2_with_ID_ptr operator() ( const Source_trisegment_2_with_ID_ptr& aT ) const
  {
    const auto& lSe0 = aT->e0();
    const auto& lSe1 = aT->e1();
    const auto& lSe2 = aT->e2();

    const Source_FT& lW0 = aT->w0();
    const Source_FT& lW1 = aT->w1();
    const Source_FT& lW2 = aT->w2();

    Trisegment_collinearity lCollinearity = aT->collinearity();
    std::size_t lId = aT->id();

    Target_trisegment_2_with_ID_ptr rT = Target_trisegment_2_with_ID_ptr(
                                           new Target_trisegment_2_with_ID(this->operator()(lSe0),
                                                                           this->operator()(lW0),
                                                                           this->operator()(lSe1),
                                                                           this->operator()(lW1),
                                                                           this->operator()(lSe2),
                                                                           this->operator()(lW2),
                                                                           lCollinearity, lId));

    if ( aT->child_l() )
      rT->set_child_l(this->operator()(aT->child_l()));
    if ( aT->child_r() )
      rT->set_child_r(this->operator()(aT->child_r()));
    if ( aT->child_t() )
      rT->set_child_t(this->operator()(aT->child_t()));

    return rT;
  }

} ;

template<class Source_skeleton_, class Target_skeleton_, class Items_converter_>
struct Straight_skeleton_converter_2
{
  typedef Source_skeleton_ Source_skeleton ;
  typedef Target_skeleton_ Target_skeleton ;
  typedef Items_converter_ Items_converter ;

  typedef typename Source_skeleton::Traits Source_traits ;
  typedef typename Target_skeleton::Traits Target_traits ;

  typedef std::shared_ptr<Target_skeleton> Target_skeleton_ptr ;

  typedef typename Source_skeleton::Vertex_const_iterator   Source_vertex_const_iterator ;
  typedef typename Source_skeleton::Halfedge_const_iterator Source_halfedge_const_iterator ;
  typedef typename Source_skeleton::Face_const_iterator     Source_face_const_iterator  ;

  typedef typename Source_skeleton::Halfedge_handle Source_halfedge_handle ;

  typedef typename Target_skeleton::Vertex   Target_vertex ;
  typedef typename Target_skeleton::Halfedge Target_halfedge ;
  typedef typename Target_skeleton::Face     Target_face ;

  typedef typename Target_skeleton::Vertex_handle   Target_vertex_handle ;
  typedef typename Target_skeleton::Halfedge_handle Target_halfedge_handle ;
  typedef typename Target_skeleton::Face_handle     Target_face_handle  ;

  typedef typename Target_skeleton::Vertex_iterator   Target_vertex_iterator ;
  typedef typename Target_skeleton::Halfedge_iterator Target_halfedge_iterator ;
  typedef typename Target_skeleton::Face_iterator     Target_face_iterator  ;

  typedef typename Target_skeleton::Base      SlsBase ;
  typedef typename Target_halfedge::Base_base HBase_base ;
  typedef typename Target_halfedge::Base      HBase ;
  typedef typename Target_vertex::Base        VBase ;
  typedef typename Target_face::Base          FBase ;

  typedef CGAL_SS_i::Triedge<Source_halfedge_handle> Source_triedge ;
  typedef CGAL_SS_i::Triedge<Target_halfedge_handle> Target_triedge ;

  Straight_skeleton_converter_2 ( Items_converter const& aCvt = Items_converter() )
    :
    cvt(aCvt)
  {}

  Target_skeleton_ptr operator() ( Source_skeleton const& aSkeleton ) const
  {
    CGAL_assertion(aSkeleton.is_valid(true /*likely partial skeleton*/));
    Target_skeleton_ptr rResult = create_unconnected_copy(aSkeleton);
    connect_items(aSkeleton,*rResult);
    CGAL_assertion(rResult->is_valid(true /*likely partial skeleton*/));
    return rResult ;
  }

private :

  Target_skeleton_ptr create_unconnected_copy ( Source_skeleton const& aSource ) const
  {
    Target_skeleton_ptr rCopy ( new Target_skeleton ) ;

    int lMaxVID =-1, lMaxHID = -1, lMaxFID = -1 ;

    for ( Source_vertex_const_iterator vit = aSource.vertices_begin(); vit != aSource.vertices_end(); ++ vit )
      if ( vit->id() > lMaxVID )
        lMaxVID = vit->id();

    for ( Source_halfedge_const_iterator hit = aSource.halfedges_begin(); hit != aSource.halfedges_end(); ++ hit )
      if ( hit->id() > lMaxHID )
        lMaxHID = hit->id();

    for ( Source_face_const_iterator fit = aSource.faces_begin(); fit != aSource.faces_end(); ++ fit )
      if ( fit->id() > lMaxFID )
        lMaxFID = fit->id();

    Target_vertices .clear();
    Target_halfedges.clear();
    Target_faces    .clear();
    Target_vertices .resize(lMaxVID+1);
    Target_halfedges.resize(lMaxHID+1);
    Target_faces    .resize(lMaxFID+1);

    for ( Source_vertex_const_iterator vit = aSource.vertices_begin(); vit != aSource.vertices_end(); ++ vit )
    {
      Target_vertex_handle tv = rCopy->SlsBase::vertices_push_back( cvt(vit) ) ;

      Target_vertices.at(tv->id()) = tv ;
    }

    for ( Source_halfedge_const_iterator hit = aSource.halfedges_begin(); hit != aSource.halfedges_end(); ++++ hit )
    {
      // In this loop, `hit` is incremented twice, to iterate on edges. We
      // could have used `edges_begin()` and `edges_end()` instead.

      Target_halfedge_handle    th = rCopy->SlsBase::edges_push_back( cvt(hit), cvt(hit->opposite()) ) ;
      Target_halfedge_handle oppth = th->opposite();

      Target_halfedges.at(   th->id()) = th ;
      Target_halfedges.at(oppth->id()) = oppth ;

    }

    for ( Source_face_const_iterator fit = aSource.faces_begin(); fit != aSource.faces_end(); ++ fit )
    {
      Target_face_handle tf = rCopy->SlsBase::faces_push_back( cvt(fit) ) ;

      Target_faces.at(tf->id()) = tf ;
    }

    return rCopy ;
  }

  void connect_items ( Source_skeleton const& aSource, Target_skeleton& aTarget ) const
  {
    Target_vertex_iterator tvit = aTarget.vertices_begin();
    for ( Source_vertex_const_iterator svit = aSource.vertices_begin(); svit != aSource.vertices_end(); ++ svit, ++ tvit )
    {
      CGAL_assertion( handle_assigned(svit) ) ;
      CGAL_assertion( handle_assigned(svit->halfedge()) ) ;

      Target_halfedge_handle tgt_halfedge = Target_halfedges.at(svit->halfedge()->id());

      CGAL_assertion( handle_assigned(tgt_halfedge) ) ;
      tvit->VBase::set_halfedge(tgt_halfedge);

      if(svit->trisegment()) // contour nodes do not have trisegments
        tvit->set_trisegment(cvt(svit->trisegment()));
    }

    Target_halfedge_iterator thit = aTarget.halfedges_begin();
    for ( Source_halfedge_const_iterator shit = aSource.halfedges_begin(); shit != aSource.halfedges_end(); ++ shit, ++ thit )
    {
      CGAL_assertion( handle_assigned(shit->opposite()) ) ;
      CGAL_assertion( handle_assigned(shit->next    ()) ) ;
      CGAL_assertion( handle_assigned(shit->prev    ()) ) ;
      CGAL_assertion( handle_assigned(shit->vertex  ()) ) ;

      Target_halfedge_handle tgt_opposite = Target_halfedges.at(shit->opposite()->id());
      Target_halfedge_handle tgt_next     = Target_halfedges.at(shit->next    ()->id());
      Target_halfedge_handle tgt_prev     = Target_halfedges.at(shit->prev    ()->id());
      Target_vertex_handle   tgt_vertex   = Target_vertices .at(shit->vertex  ()->id());

      CGAL_assertion( handle_assigned(tgt_opposite) ) ;
      CGAL_assertion( handle_assigned(tgt_next)     ) ;
      CGAL_assertion( handle_assigned(tgt_prev)     ) ;
      CGAL_assertion( handle_assigned(tgt_vertex)   ) ;

      thit->HBase_base::set_opposite (tgt_opposite);
      thit->HBase_base::set_next     (tgt_next);
      thit->HBase_base::set_prev     (tgt_prev);
      thit->HBase_base::set_vertex   (tgt_vertex);

      if ( handle_assigned(shit->face()) )
      {
        Target_face_handle tgt_face = Target_faces.at(shit->face()->id());
        CGAL_assertion( handle_assigned(tgt_face) ) ;
        thit->HBase_base::set_face(tgt_face);
      }
     }

    Target_face_iterator tfit = aTarget.faces_begin();
    for ( Source_face_const_iterator sfit = aSource.faces_begin(); sfit != aSource.faces_end(); ++ sfit, ++ tfit )
    {
      CGAL_assertion( handle_assigned(sfit->halfedge()) ) ;

      Target_halfedge_handle tgt_halfedge = Target_halfedges.at(sfit->halfedge()->id());

      CGAL_assertion( handle_assigned(tgt_halfedge) ) ;

      tfit->FBase::set_halfedge(tgt_halfedge);
    }
  }

  Items_converter const& cvt ;

  mutable std::vector<Target_vertex_handle>   Target_vertices ;
  mutable std::vector<Target_halfedge_handle> Target_halfedges;
  mutable std::vector<Target_face_handle>     Target_faces    ;

} ;

template<class Target_skeleton, class Source_skeleton, class Items_converter>
std::shared_ptr<Target_skeleton>
convert_straight_skeleton_2 ( Source_skeleton const& aSrc, Items_converter const& ic )
{
  typedef Straight_skeleton_converter_2<Source_skeleton,Target_skeleton,Items_converter> Skeleton_converter ;

  Skeleton_converter c(ic) ;

  return c(aSrc);

}

template<class Target_skeleton, class Source_skeleton>
std::shared_ptr<Target_skeleton>
convert_straight_skeleton_2 ( Source_skeleton const& aSrc )
{
  typedef Straight_skeleton_items_converter_2<Source_skeleton,Target_skeleton> Items_converter ;

  typedef Straight_skeleton_converter_2<Source_skeleton,Target_skeleton,Items_converter> Skeleton_converter ;

  Skeleton_converter c ;

  return c(aSrc);

}

} // end namespace CGAL

#endif // CGAL_STRAIGHT_SKELETON_2_CONVERTER_H //
// EOF //

