add_subdirectory(energyMinimization)
add_subdirectory(graph)
add_subdirectory(regionGrowing)
add_subdirectory(ransac)

# GUI version
add_executable(${PROJECT_NAME}_GUI main_gui.cpp functions.cpp viewer_gui.cpp)

target_include_directories(${PROJECT_NAME}_GUI PRIVATE 
    ${Thesis_EXTERNAL_DIR}
    ${CMAKE_CURRENT_LIST_DIR}
)

# find CGAL
set(CGAL_DIR ${Thesis_EXTERNAL_DIR}/CGAL)
find_package(CGAL REQUIRED PATHS ${CGAL_DIR})

# Common libraries
set(COMMON_LIBRARIES
    energyMinimization
    graph
    regionGrowing
    ransac
    
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

target_link_libraries(${PROJECT_NAME}_GUI 
    ${COMMON_LIBRARIES}
    3rd_imgui  # Add ImGui for GUI version
)
