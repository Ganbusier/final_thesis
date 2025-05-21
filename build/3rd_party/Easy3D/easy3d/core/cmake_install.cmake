# Install script for directory: D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/core

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
     "C:/Program Files/Easy3D/2.5.4/lib/easy3d_core.lib")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    file(INSTALL DESTINATION "C:/Program Files/Easy3D/2.5.4/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Files/GitHub/final_thesis/lib/Debug/easy3d_core.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files/Easy3D/2.5.4/lib/easy3d_core.lib")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    file(INSTALL DESTINATION "C:/Program Files/Easy3D/2.5.4/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Files/GitHub/final_thesis/lib/Release/easy3d_core.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files/Easy3D/2.5.4/lib/easy3d_core.lib")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    file(INSTALL DESTINATION "C:/Program Files/Easy3D/2.5.4/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Files/GitHub/final_thesis/lib/MinSizeRel/easy3d_core.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files/Easy3D/2.5.4/lib/easy3d_core.lib")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    file(INSTALL DESTINATION "C:/Program Files/Easy3D/2.5.4/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Files/GitHub/final_thesis/lib/RelWithDebInfo/easy3d_core.lib")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "bin" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files/Easy3D/2.5.4/bin/easy3d_core.dll")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    file(INSTALL DESTINATION "C:/Program Files/Easy3D/2.5.4/bin" TYPE SHARED_LIBRARY FILES "D:/Files/GitHub/final_thesis/bin/Debug/easy3d_core.dll")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files/Easy3D/2.5.4/bin/easy3d_core.dll")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    file(INSTALL DESTINATION "C:/Program Files/Easy3D/2.5.4/bin" TYPE SHARED_LIBRARY FILES "D:/Files/GitHub/final_thesis/bin/Release/easy3d_core.dll")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files/Easy3D/2.5.4/bin/easy3d_core.dll")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    file(INSTALL DESTINATION "C:/Program Files/Easy3D/2.5.4/bin" TYPE SHARED_LIBRARY FILES "D:/Files/GitHub/final_thesis/bin/MinSizeRel/easy3d_core.dll")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files/Easy3D/2.5.4/bin/easy3d_core.dll")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    file(INSTALL DESTINATION "C:/Program Files/Easy3D/2.5.4/bin" TYPE SHARED_LIBRARY FILES "D:/Files/GitHub/final_thesis/bin/RelWithDebInfo/easy3d_core.dll")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "dev" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "C:/Program Files/Easy3D/2.5.4/include/easy3d/core/box.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/core/constant.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/core/curve.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/core/eigen_solver.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/core/graph.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/core/hash.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/core/heap.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/core/line.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/core/surface_mesh_builder.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/core/mat.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/core/matrix.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/core/matrix_algo.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/core/model.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/core/oriented_line.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/core/plane.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/core/point_cloud.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/core/principal_axes.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/core/property.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/core/quat.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/core/random.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/core/rect.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/core/segment.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/core/signal.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/core/spline_curve_fitting.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/core/spline_curve_interpolation.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/core/spline_interpolation.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/core/surface_mesh.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/core/poly_mesh.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/core/polygon.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/core/types.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/core/vec.h")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "C:/Program Files/Easy3D/2.5.4/include/easy3d/core" TYPE FILE FILES
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/core/box.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/core/constant.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/core/curve.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/core/eigen_solver.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/core/graph.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/core/hash.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/core/heap.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/core/line.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/core/surface_mesh_builder.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/core/mat.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/core/matrix.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/core/matrix_algo.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/core/model.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/core/oriented_line.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/core/plane.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/core/point_cloud.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/core/principal_axes.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/core/property.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/core/quat.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/core/random.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/core/rect.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/core/segment.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/core/signal.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/core/spline_curve_fitting.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/core/spline_curve_interpolation.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/core/spline_interpolation.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/core/surface_mesh.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/core/poly_mesh.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/core/polygon.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/core/types.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/core/vec.h"
    )
endif()

