get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)

set(${PROJECT_NAME}_HEADERS
    ./ransac3d.h
    ./ransac3d2d.h
    ./CustomCylinderConstructor.h
)

set(${PROJECT_NAME}_SOURCES
    ./ransac3d.cpp
    ./ransac3d2d.cpp
    ./CustomCylinderConstructor.cpp
)

add_library(${PROJECT_NAME} STATIC ${${PROJECT_NAME}_SOURCES} ${${PROJECT_NAME}_HEADERS})

# add 3rd party path
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${Thesis_EXTERNAL_DIR}
    ${Thesis_EXTERNAL_DIR}/efficientRansac
)

# add 3rd party libraries
target_link_libraries(${PROJECT_NAME} PRIVATE 
    CGAL::CGAL
    easy3d::core
    easy3d::algo
    efficientRansac
)

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "src")

