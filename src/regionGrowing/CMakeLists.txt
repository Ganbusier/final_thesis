get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)

set(${PROJECT_NAME}_HEADERS
    ./regionGrowing.h
    ./cgalDefines_rg.h
)

set(${PROJECT_NAME}_SOURCES
    ./regionGrowing.cpp
)

add_library(${PROJECT_NAME} STATIC ${${PROJECT_NAME}_SOURCES} ${${PROJECT_NAME}_HEADERS})

# add 3rd party path
target_include_directories(${PROJECT_NAME} PRIVATE ${Thesis_EXTERNAL_DIR})

# add cgal and easy3d
target_link_libraries(${PROJECT_NAME} PRIVATE 
    CGAL::CGAL
    easy3d::core
)

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "src")

