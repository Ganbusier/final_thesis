add_subdirectory(energyMinimization)
add_subdirectory(graph)
add_subdirectory(regionGrowing)

add_executable(${PROJECT_NAME} main.cpp functions.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${Thesis_EXTERNAL_DIR}
    ${CMAKE_CURRENT_LIST_DIR}
)

# find CGAL
set(CGAL_DIR ${Thesis_EXTERNAL_DIR}/CGAL)
find_package(CGAL REQUIRED PATHS ${CGAL_DIR})

target_link_libraries(${PROJECT_NAME} 
    energyMinimization
    graph
    regionGrowing

    gco
    CGAL::CGAL
    easy3d::viewer
    easy3d::renderer
    easy3d::algo
    easy3d::core
    easy3d::fileio
    easy3d::util
    easy3d::kdtree
)
