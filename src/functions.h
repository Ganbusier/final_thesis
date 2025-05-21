#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <easy3d/core/model.h>
#include <easy3d/viewer/viewer.h>
#include <energyMinimization/energyMinimization.h>
#include <graph/graph.h>
#include <regionGrowing/regionGrowing.h>
#include <myRansac/ransac3d.h>

#include "cgalDefines.h"


bool run_EnergyMinimization(easy3d::Viewer* viewer, easy3d::Model* model,
                            const std::string& saveFilePath);

bool run_RegionGrowing(easy3d::Viewer* viewer, easy3d::Model* model,
                       const std::string& saveFilePath);

bool run_Ransac3d(easy3d::Viewer* viewer, easy3d::Model* model,
                  const std::string& saveFilePath);

#endif  // FUNCTIONS_H
