// Copyright (c) 2008       GeometryFactory (France)
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
//
// $URL: https://github.com/CGAL/cgal/blob/v6.0.1/Surface_mesher/include/CGAL/Surface_mesher/Profile_timer.h $
// $Id: include/CGAL/Surface_mesher/Profile_timer.h 50cfbde3b84 $
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-Commercial
//
//
// Author(s)     : Laurent Rineau

// This file is an adaptation of <CGAL/Profile_timer.h>, so that the
// macros are prefixed with CGAL_SURFACE_MESHER_ instead of CGAL_.

#ifndef CGAL_SURFACE_MESHER_PROFILE_TIMER_H
#define CGAL_SURFACE_MESHER_PROFILE_TIMER_H

#include <CGAL/license/Surface_mesher.h>

#define CGAL_DEPRECATED_HEADER "<CGAL/Surface_mesher/Profile_timer.h>"
#define CGAL_DEPRECATED_MESSAGE_DETAILS \
  "The 3D Mesh Generation package (see https://doc.cgal.org/latest/Mesh_3/) should be used instead."
#include <CGAL/Installation/internal/deprecation_warning.h>

#include <CGAL/Profile_timer.h>

#ifdef CGAL_SURFACE_MESHER_PROFILE
#  define CGAL_SURFACE_MESHER_TIME_PROFILER(NAME) \
          static CGAL::Profile_timer CGAL_profile_timer_tmp(NAME); \
          CGAL::Profile_timer::Local CGAL_local_profile_timer_tmp(&CGAL_profile_timer_tmp);
#else
#  define CGAL_SURFACE_MESHER_TIME_PROFILER(NAME)
#endif

#endif // CGAL_SURFACE_MESHER_TIME_PROFILER
