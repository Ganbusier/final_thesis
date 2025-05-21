# Install script for directory: D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/renderer

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
     "C:/Program Files/Easy3D/2.5.4/lib/easy3d_renderer.lib")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    file(INSTALL DESTINATION "C:/Program Files/Easy3D/2.5.4/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Files/GitHub/final_thesis/lib/Debug/easy3d_renderer.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files/Easy3D/2.5.4/lib/easy3d_renderer.lib")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    file(INSTALL DESTINATION "C:/Program Files/Easy3D/2.5.4/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Files/GitHub/final_thesis/lib/Release/easy3d_renderer.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files/Easy3D/2.5.4/lib/easy3d_renderer.lib")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    file(INSTALL DESTINATION "C:/Program Files/Easy3D/2.5.4/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Files/GitHub/final_thesis/lib/MinSizeRel/easy3d_renderer.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files/Easy3D/2.5.4/lib/easy3d_renderer.lib")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    file(INSTALL DESTINATION "C:/Program Files/Easy3D/2.5.4/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Files/GitHub/final_thesis/lib/RelWithDebInfo/easy3d_renderer.lib")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "bin" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files/Easy3D/2.5.4/bin/easy3d_renderer.dll")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    file(INSTALL DESTINATION "C:/Program Files/Easy3D/2.5.4/bin" TYPE SHARED_LIBRARY FILES "D:/Files/GitHub/final_thesis/bin/Debug/easy3d_renderer.dll")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files/Easy3D/2.5.4/bin/easy3d_renderer.dll")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    file(INSTALL DESTINATION "C:/Program Files/Easy3D/2.5.4/bin" TYPE SHARED_LIBRARY FILES "D:/Files/GitHub/final_thesis/bin/Release/easy3d_renderer.dll")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files/Easy3D/2.5.4/bin/easy3d_renderer.dll")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    file(INSTALL DESTINATION "C:/Program Files/Easy3D/2.5.4/bin" TYPE SHARED_LIBRARY FILES "D:/Files/GitHub/final_thesis/bin/MinSizeRel/easy3d_renderer.dll")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files/Easy3D/2.5.4/bin/easy3d_renderer.dll")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    file(INSTALL DESTINATION "C:/Program Files/Easy3D/2.5.4/bin" TYPE SHARED_LIBRARY FILES "D:/Files/GitHub/final_thesis/bin/RelWithDebInfo/easy3d_renderer.dll")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "dev" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "C:/Program Files/Easy3D/2.5.4/include/easy3d/renderer/ambient_occlusion.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/renderer/average_color_blending.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/renderer/camera.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/renderer/clipping_plane.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/renderer/constraint.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/renderer/drawable.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/renderer/drawable_lines.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/renderer/drawable_points.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/renderer/drawable_triangles.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/renderer/dual_depth_peeling.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/renderer/eye_dome_lighting.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/renderer/frame.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/renderer/framebuffer_object.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/renderer/frustum.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/renderer/key_frame_interpolator.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/renderer/manipulator.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/renderer/manipulated_camera_frame.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/renderer/manipulated_frame.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/renderer/opengl.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/renderer/opengl_error.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/renderer/opengl_util.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/renderer/opengl_timer.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/renderer/shape.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/renderer/read_pixel.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/renderer/buffer.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/renderer/renderer.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/renderer/shader_manager.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/renderer/shader_program.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/renderer/shadow.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/renderer/soft_shadow.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/renderer/state.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/renderer/texture.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/renderer/texture_manager.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/renderer/text_renderer.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/renderer/transform.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/renderer/transform_decompose.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/renderer/transparency.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/renderer/vertex_array_object.h")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "C:/Program Files/Easy3D/2.5.4/include/easy3d/renderer" TYPE FILE FILES
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/renderer/ambient_occlusion.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/renderer/average_color_blending.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/renderer/camera.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/renderer/clipping_plane.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/renderer/constraint.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/renderer/drawable.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/renderer/drawable_lines.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/renderer/drawable_points.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/renderer/drawable_triangles.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/renderer/dual_depth_peeling.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/renderer/eye_dome_lighting.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/renderer/frame.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/renderer/framebuffer_object.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/renderer/frustum.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/renderer/key_frame_interpolator.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/renderer/manipulator.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/renderer/manipulated_camera_frame.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/renderer/manipulated_frame.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/renderer/opengl.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/renderer/opengl_error.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/renderer/opengl_util.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/renderer/opengl_timer.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/renderer/shape.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/renderer/read_pixel.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/renderer/buffer.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/renderer/renderer.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/renderer/shader_manager.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/renderer/shader_program.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/renderer/shadow.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/renderer/soft_shadow.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/renderer/state.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/renderer/texture.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/renderer/texture_manager.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/renderer/text_renderer.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/renderer/transform.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/renderer/transform_decompose.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/renderer/transparency.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/renderer/vertex_array_object.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "C:/Program Files/Easy3D/2.5.4/include/3rd_party/glew/include/GL/glew.h")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "C:/Program Files/Easy3D/2.5.4/include/3rd_party/glew/include/GL" TYPE FILE FILES "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/3rd_party/glew/include/GL/glew.h")
endif()

