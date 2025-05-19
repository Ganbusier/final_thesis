#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <easy3d/fileio/point_cloud_io.h>
#include <easy3d/core/point_cloud.h>
#include <energyMinimization/energyMinimization.h>
#include <graph/graph.h>

void run_EnergyMinimization(easy3d::PointCloud* pointCloud, const std::string& saveFilePath);


#endif  // FUNCTIONS_H

