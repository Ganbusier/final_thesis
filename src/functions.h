#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <easy3d/core/model.h>
#include <easy3d/viewer/viewer.h>
#include <easy3d/renderer/drawable.h>
#include <vector>

// External declaration of the global drawables vector
extern std::vector<easy3d::Drawable*> drawables;

bool run_EnergyMinimization(easy3d::Viewer* viewer, easy3d::Model* model,
                            const std::string& saveFilePath);

bool run_RegionGrowing(easy3d::Viewer* viewer, easy3d::Model* model,
                       const std::string& saveFilePath);

bool run_Ransac3d(easy3d::Viewer* viewer, easy3d::Model* model,
                  const std::string& saveFilePath);

bool run_Ransac3d2d(easy3d::Viewer* viewer, easy3d::Model* model,
                    const std::string& saveFilePath);

// Function to clear all algorithm-generated drawables
void clear_algorithm_drawables();

// Function to set the color for newly created line drawables
void set_line_drawable_color(const easy3d::vec4& color);

// Function to get the current line color
easy3d::vec4 get_current_line_color();

#endif  // FUNCTIONS_H
