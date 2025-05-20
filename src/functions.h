#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <easy3d/core/model.h>
#include <easy3d/core/point_cloud.h>
#include <easy3d/fileio/point_cloud_io.h>
#include <easy3d/renderer/camera.h>
#include <easy3d/renderer/drawable_points.h>
#include <easy3d/renderer/renderer.h>
#include <easy3d/util/initializer.h>
#include <easy3d/viewer/viewer.h>
#include <energyMinimization/energyMinimization.h>
#include <graph/graph.h>


bool run_EnergyMinimization(easy3d::Viewer* viewer, easy3d::Model* model,
                            const std::string& saveFilePath);

#endif  // FUNCTIONS_H
