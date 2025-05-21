
#ifndef EASY3D_KDTREE_EXPORT_H
#define EASY3D_KDTREE_EXPORT_H

#ifdef EASY3D_KDTREE_STATIC_DEFINE
#  define EASY3D_KDTREE_EXPORT
#  define EASY3D_KDTREE_NO_EXPORT
#else
#  ifndef EASY3D_KDTREE_EXPORT
#    ifdef easy3d_kdtree_EXPORTS
        /* We are building this library */
#      define EASY3D_KDTREE_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define EASY3D_KDTREE_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef EASY3D_KDTREE_NO_EXPORT
#    define EASY3D_KDTREE_NO_EXPORT 
#  endif
#endif

#ifndef EASY3D_KDTREE_DEPRECATED
#  define EASY3D_KDTREE_DEPRECATED __declspec(deprecated)
#endif

#ifndef EASY3D_KDTREE_DEPRECATED_EXPORT
#  define EASY3D_KDTREE_DEPRECATED_EXPORT EASY3D_KDTREE_EXPORT EASY3D_KDTREE_DEPRECATED
#endif

#ifndef EASY3D_KDTREE_DEPRECATED_NO_EXPORT
#  define EASY3D_KDTREE_DEPRECATED_NO_EXPORT EASY3D_KDTREE_NO_EXPORT EASY3D_KDTREE_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef EASY3D_KDTREE_NO_DEPRECATED
#    define EASY3D_KDTREE_NO_DEPRECATED
#  endif
#endif

#endif /* EASY3D_KDTREE_EXPORT_H */
