add_subdirectory(energyMinimization)
add_subdirectory(graph)
add_subdirectory(regionGrowing)
add_subdirectory(ransac)

# GUI version
add_executable(${PROJECT_NAME}_GUI main_gui.cpp functions.cpp viewer_gui.cpp)

# Analysis version
add_executable(${PROJECT_NAME}_Analysis main_analysis.cpp analysis.cpp)

target_include_directories(${PROJECT_NAME}_GUI PRIVATE 
    ${Thesis_EXTERNAL_DIR}
    ${CMAKE_CURRENT_LIST_DIR}
)

target_include_directories(${PROJECT_NAME}_Analysis PRIVATE 
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
    # Add ImGui for GUI version, this if from Easy3D
    3rd_imgui  
)

target_link_libraries(${PROJECT_NAME}_Analysis 
    ${COMMON_LIBRARIES}
)
