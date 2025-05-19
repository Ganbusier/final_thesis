get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)

set(${PROJECT_NAME}_HEADERS
    ./energyMinimization.h
)

set(${PROJECT_NAME}_SOURCES
    ./energyMinimization.cpp
)

add_library(${PROJECT_NAME} STATIC ${${PROJECT_NAME}_SOURCES} ${${PROJECT_NAME}_HEADERS})

# add 3rd party path
target_include_directories(${PROJECT_NAME} PRIVATE ${Thesis_EXTERNAL_DIR})

# add easy3d and gco
target_link_libraries(${PROJECT_NAME} PRIVATE 
    easy3d::core
    gco
)

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "src")

