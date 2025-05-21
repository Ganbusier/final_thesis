
#ifndef EASY3D_VIEWER_EXPORT_H
#define EASY3D_VIEWER_EXPORT_H

#ifdef EASY3D_VIEWER_STATIC_DEFINE
#  define EASY3D_VIEWER_EXPORT
#  define EASY3D_VIEWER_NO_EXPORT
#else
#  ifndef EASY3D_VIEWER_EXPORT
#    ifdef easy3d_viewer_EXPORTS
        /* We are building this library */
#      define EASY3D_VIEWER_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define EASY3D_VIEWER_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef EASY3D_VIEWER_NO_EXPORT
#    define EASY3D_VIEWER_NO_EXPORT 
#  endif
#endif

#ifndef EASY3D_VIEWER_DEPRECATED
#  define EASY3D_VIEWER_DEPRECATED __declspec(deprecated)
#endif

#ifndef EASY3D_VIEWER_DEPRECATED_EXPORT
#  define EASY3D_VIEWER_DEPRECATED_EXPORT EASY3D_VIEWER_EXPORT EASY3D_VIEWER_DEPRECATED
#endif

#ifndef EASY3D_VIEWER_DEPRECATED_NO_EXPORT
#  define EASY3D_VIEWER_DEPRECATED_NO_EXPORT EASY3D_VIEWER_NO_EXPORT EASY3D_VIEWER_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef EASY3D_VIEWER_NO_DEPRECATED
#    define EASY3D_VIEWER_NO_DEPRECATED
#  endif
#endif

#endif /* EASY3D_VIEWER_EXPORT_H */
