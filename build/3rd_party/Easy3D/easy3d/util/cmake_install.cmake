# Install script for directory: D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/util

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
     "C:/Program Files/Easy3D/2.5.4/lib/easy3d_util.lib")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    file(INSTALL DESTINATION "C:/Program Files/Easy3D/2.5.4/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Files/GitHub/final_thesis/lib/Debug/easy3d_util.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files/Easy3D/2.5.4/lib/easy3d_util.lib")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    file(INSTALL DESTINATION "C:/Program Files/Easy3D/2.5.4/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Files/GitHub/final_thesis/lib/Release/easy3d_util.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files/Easy3D/2.5.4/lib/easy3d_util.lib")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    file(INSTALL DESTINATION "C:/Program Files/Easy3D/2.5.4/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Files/GitHub/final_thesis/lib/MinSizeRel/easy3d_util.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files/Easy3D/2.5.4/lib/easy3d_util.lib")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    file(INSTALL DESTINATION "C:/Program Files/Easy3D/2.5.4/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "D:/Files/GitHub/final_thesis/lib/RelWithDebInfo/easy3d_util.lib")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "bin" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files/Easy3D/2.5.4/bin/easy3d_util.dll")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    file(INSTALL DESTINATION "C:/Program Files/Easy3D/2.5.4/bin" TYPE SHARED_LIBRARY FILES "D:/Files/GitHub/final_thesis/bin/Debug/easy3d_util.dll")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files/Easy3D/2.5.4/bin/easy3d_util.dll")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    file(INSTALL DESTINATION "C:/Program Files/Easy3D/2.5.4/bin" TYPE SHARED_LIBRARY FILES "D:/Files/GitHub/final_thesis/bin/Release/easy3d_util.dll")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files/Easy3D/2.5.4/bin/easy3d_util.dll")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    file(INSTALL DESTINATION "C:/Program Files/Easy3D/2.5.4/bin" TYPE SHARED_LIBRARY FILES "D:/Files/GitHub/final_thesis/bin/MinSizeRel/easy3d_util.dll")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "C:/Program Files/Easy3D/2.5.4/bin/easy3d_util.dll")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
      message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    file(INSTALL DESTINATION "C:/Program Files/Easy3D/2.5.4/bin" TYPE SHARED_LIBRARY FILES "D:/Files/GitHub/final_thesis/bin/RelWithDebInfo/easy3d_util.dll")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "dev" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "C:/Program Files/Easy3D/2.5.4/include/easy3d/util/console_style.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/util/dialog.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/util/file_system.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/util/initializer.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/util/line_stream.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/util/logging.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/util/progress.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/util/resource.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/util/setting.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/util/stop_watch.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/util/string.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/util/timer.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/util/tokenizer.h;C:/Program Files/Easy3D/2.5.4/include/easy3d/util/version.h")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "C:/Program Files/Easy3D/2.5.4/include/easy3d/util" TYPE FILE FILES
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/util/console_style.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/util/dialog.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/util/file_system.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/util/initializer.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/util/line_stream.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/util/logging.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/util/progress.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/util/resource.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/util/setting.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/util/stop_watch.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/util/string.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/util/timer.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/util/tokenizer.h"
    "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/easy3d/util/version.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "C:/Program Files/Easy3D/2.5.4/include/3rd_party/easyloggingpp/easylogging++.h")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  file(INSTALL DESTINATION "C:/Program Files/Easy3D/2.5.4/include/3rd_party/easyloggingpp" TYPE FILE FILES "D:/Files/GitHub/final_thesis/3rd_party/Easy3D/3rd_party/easyloggingpp/easylogging++.h")
endif()

