# Install script for directory: D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/algo

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files/Easy3D/2.5.4")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "lib" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files/Easy3D/2.5.4/lib/easy3d_algo.lib")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    file(INSTALL DESTINATION "C:/Program Files/Easy3D/2.5.4/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Files/GitHub/final_thesis/lib/Debug/easy3d_algo.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files/Easy3D/2.5.4/lib/easy3d_algo.lib")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    file(INSTALL DESTINATION "C:/Program Files/Easy3D/2.5.4/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Files/GitHub/final_thesis/lib/Release/easy3d_algo.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files/Easy3D/2.5.4/lib/easy3d_algo.lib")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    file(INSTALL DESTINATION "C:/Program Files/Easy3D/2.5.4/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Files/GitHub/final_thesis/lib/MinSizeRel/easy3d_algo.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files/Easy3D/2.5.4/lib/easy3d_algo.lib")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    file(INSTALL DESTINATION "C:/Program Files/Easy3D/2.5.4/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Files/GitHub/final_thesis/lib/RelWithDebInfo/easy3d_algo.lib")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "bin" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files/Easy3D/2.5.4/bin/easy3d_algo.dll")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    file(INSTALL DESTINATION "C:/Program Files/Easy3D/2.5.4/bin" TYPE SHARED_LIBRARY FILES "D:/Files/GitHub/final_thesis/bin/Debug/easy3d_algo.dll")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files/Easy3D/2.5.4/bin/easy3d_algo.dll")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    file(INSTALL DESTINATION "C:/Program Files/Easy3D/2.5.4/bin" TYPE SHARED_LIBRARY FILES "D:/Files/GitHub/final_thesis/bin/Release/easy3d_algo.dll")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files/Easy3D/2.5.4/bin/easy3d_algo.dll")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    file(INSTALL DESTINATION "C:/Program Files/Easy3D/2.5.4/bin" TYPE SHARED_LIBRARY FILES "D:/Files/GitHub/final_thesis/bin/MinSizeRel/easy3d_algo.dll")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files/Easy3D/2.5.4/bin/easy3d_algo.dll")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    file(INSTALL DESTINATION "C:/Program Files/Easy3D/2.5.4/bin" TYPE SHARED_LIBRARY FILES "D:/Files/GitHub/final_thesis/bin/RelWithDebInfo/easy3d_algo.dll")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "dev" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "C:/Program Files/Easy3D/2.5.4/include/easy3d/algo/collider.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/algo/delaunay.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/algo/delaunay_2d.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/algo/delaunay_3d.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/algo/extrusion.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/algo/surface_mesh_geometry.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/algo/gaussian_noise.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/algo/point_cloud_normals.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/algo/point_cloud_poisson_reconstruction.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/algo/point_cloud_ransac.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/algo/point_cloud_simplification.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/algo/polygon_partition.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/algo/surface_mesh_components.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/algo/surface_mesh_curvature.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/algo/surface_mesh_enumerator.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/algo/surface_mesh_factory.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/algo/surface_mesh_fairing.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/algo/surface_mesh_features.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/algo/surface_mesh_geodesic.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/algo/surface_mesh_hole_filling.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/algo/surface_mesh_parameterization.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/algo/surface_mesh_polygonization.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/algo/surface_mesh_remeshing.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/algo/surface_mesh_sampler.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/algo/surface_mesh_simplification.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/algo/surface_mesh_smoothing.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/algo/surface_mesh_stitching.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/algo/surface_mesh_subdivision.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/algo/surface_mesh_tetrahedralization.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/algo/surface_mesh_topology.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/algo/surface_mesh_triangulation.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/algo/tessellator.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/algo/text_mesher.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/algo/triangle_mesh_kdtree.h")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "C:/Program Files/Easy3D/2.5.4/include/easy3d/algo" TYPE FILE FILES
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/algo/collider.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/algo/delaunay.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/algo/delaunay_2d.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/algo/delaunay_3d.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/algo/extrusion.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/algo/surface_mesh_geometry.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/algo/gaussian_noise.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/algo/point_cloud_normals.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/algo/point_cloud_poisson_reconstruction.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/algo/point_cloud_ransac.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/algo/point_cloud_simplification.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/algo/polygon_partition.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/algo/surface_mesh_components.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/algo/surface_mesh_curvature.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/algo/surface_mesh_enumerator.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/algo/surface_mesh_factory.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/algo/surface_mesh_fairing.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/algo/surface_mesh_features.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/algo/surface_mesh_geodesic.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/algo/surface_mesh_hole_filling.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/algo/surface_mesh_parameterization.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/algo/surface_mesh_polygonization.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/algo/surface_mesh_remeshing.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/algo/surface_mesh_sampler.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/algo/surface_mesh_simplification.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/algo/surface_mesh_smoothing.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/algo/surface_mesh_stitching.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/algo/surface_mesh_subdivision.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/algo/surface_mesh_tetrahedralization.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/algo/surface_mesh_topology.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/algo/surface_mesh_triangulation.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/algo/tessellator.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/algo/text_mesher.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/algo/triangle_mesh_kdtree.h"
    )
endif()

