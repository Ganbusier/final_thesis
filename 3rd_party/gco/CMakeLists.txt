cmake_minimum_required(VERSION 3.15)

get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${PROJECT_NAME})

set(${PROJECT_NAME}_HEADERS
    ./block.h
    ./energy.h
    ./GCoptimization.h
    ./graph.h
    ./LinkedBlockList.h
)

set(${PROJECT_NAME}_SOURCES
    ./GCoptimization.cpp
    ./LinkedBlockList.cpp
)

add_library(${PROJECT_NAME} STATIC ${${PROJECT_NAME}_SOURCES} ${${PROJECT_NAME}_HEADERS})

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "3rd_party")



