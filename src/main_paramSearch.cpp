#include <easy3d/core/graph.h>
#include <easy3d/core/point_cloud.h>
#include <easy3d/fileio/graph_io.h>
#include <easy3d/fileio/point_cloud_io.h>
#include <energyMinimization/energyMinimization.h>
#include <graph/graph.h>
#include <ransac/ransac3d.h>
#include <ransac/ransac3d2d.h>
#include <regionGrowing/cgalDefines_rg.h>
#include <regionGrowing/regionGrowing.h>

#include <iostream>

#include "analysis.h"
#include "logging_config.h"

// Function declaration
void ransac3d_paramSearch(easy3d::PointCloud* pointCloud);
void ransac3d2d_paramSearch(easy3d::PointCloud* pointCloud);
void regionGrowing_paramSearch(easy3d::PointCloud* pointCloud);
void energyMinimization_paramSearch(easy3d::PointCloud* pointCloud);

int main(int argc, char** argv) {
  // Configure logging to suppress logging from other libraries
  configure_logging_for_main_param_search();

  if (argc != 3) {
    std::cout << "ERROR: Usage: " << argv[0]
              << " <input_pointcloud_ply_file> <parameter_search_type (0: "
                 "ransac3d, 1: ransac3d2d, 2: regionGrowing, 3: all)>"
              << std::endl;
    return 1;
  }
  std::string estimatedFile = argv[1];
  int parameterSearchType = std::stoi(argv[2]);

  easy3d::PointCloud* pointCloud = easy3d::PointCloudIO::load(estimatedFile);

  if (parameterSearchType == 0) {
    ransac3d_paramSearch(pointCloud);
  } else if (parameterSearchType == 1) {
    ransac3d2d_paramSearch(pointCloud);
  } else if (parameterSearchType == 2) {
    regionGrowing_paramSearch(pointCloud);
  } else if (parameterSearchType == 3) {
    ransac3d_paramSearch(pointCloud);
    ransac3d2d_paramSearch(pointCloud);
    regionGrowing_paramSearch(pointCloud);

    return 0;
  }
}

void ransac3d_paramSearch(easy3d::PointCloud* pointCloud) {
  ransac::Params best_params;
  float best_score = 1e20;
  int best_leftoverPoints = 1e20;
  int best_numPrimitives = 0;

  float weight_primitives = 1.0;
  float weight_leftovers = 1.0;

  float probability = 0.01;
  float minRadius = 0.01;
  float maxRadius = 1.0;
  std::vector<float> normalThresholds = {0.0, 0.3, 0.5, 0.7, 0.9};
  std::vector<float> epsilonValues = {0.01, 0.02, 0.03, 0.04, 0.05};
  std::vector<float> clusterEpsilonValues = {0.1, 0.3, 0.5, 0.7, 0.9,
                                             1.0, 1.3, 1.5, 2.0};
  std::vector<int> minPointsValues = {5, 10, 15, 20, 25, 30, 35, 40, 45, 50};

  int total_combinations = normalThresholds.size() * epsilonValues.size() *
                           clusterEpsilonValues.size() * minPointsValues.size();
  std::cout << "Starting parameter search for Ransac3D with "
            << total_combinations << " combinations..." << std::endl;
  std::cout << "Optimization targets: maximize primitives (weight="
            << weight_primitives
            << ") + minimize leftover points (weight=" << weight_leftovers
            << ")" << std::endl;

  int current_combination = 0;
  int progress_milestone = total_combinations / 10;  // 10% increments

  for (float normalThreshold : normalThresholds) {
    for (float epsilon : epsilonValues) {
      for (float clusterEpsilon : clusterEpsilonValues) {
        for (int minPoints : minPointsValues) {
          current_combination++;

          // Progress reporting every 10%
          if (current_combination % progress_milestone == 0 ||
              current_combination == total_combinations) {
            int progress_percent =
                (current_combination * 100) / total_combinations;
            std::cout << "Progress: " << progress_percent << "% ("
                      << current_combination << "/" << total_combinations
                      << " combinations completed)" << std::endl;
          }

          ransac::Params params;
          params.normalThreshold = normalThreshold;
          params.probability = probability;
          params.minRadius = minRadius;
          params.maxRadius = maxRadius;
          params.epsilon = epsilon;
          params.clusterEpsilon = clusterEpsilon;
          params.minPoints = minPoints;

          ransac::Ransac3d ransac3d(pointCloud, params);
          ransac3d.detect();

          std::vector<ransac::CylinderInfo> cylinderInfos =
              ransac3d.getCylinderInfos();
          std::vector<int> leftovers = ransac3d.getLeftoverIndices();

          int numPrimitives = cylinderInfos.size();
          int leftoverPoints = leftovers.size();

          // Objective: maximize number of primitives, minimize leftover
          // points score = penalty for leftover points - reward for
          // primitives
          float score = weight_leftovers * leftoverPoints -
                        weight_primitives * numPrimitives;

          if (score < best_score) {
            best_score = score;
            best_leftoverPoints = leftoverPoints;
            best_numPrimitives = numPrimitives;
            best_params = params;
          }
        }
      }
    }
  }

  std::cout << "\n=== Parameter Search Results ===" << std::endl;
  std::cout << "Best parameters for Ransac3D: "
            << "normalThreshold: " << best_params.normalThreshold << ", "
            << "epsilon: " << best_params.epsilon << ", "
            << "clusterEpsilon: " << best_params.clusterEpsilon << ", "
            << "minPoints: " << best_params.minPoints << ", "
            << "probability: " << best_params.probability << ", "
            << "minRadius: " << best_params.minRadius << ", "
            << "maxRadius: " << best_params.maxRadius << std::endl;
  std::cout << "Number of primitives: " << best_numPrimitives << std::endl;
  std::cout << "Number of leftover points: " << best_leftoverPoints
            << std::endl;
  std::cout << "Combined score: " << best_score << std::endl;
  std::cout << "=== Search Complete ===" << std::endl;
}

void ransac3d2d_paramSearch(easy3d::PointCloud* pointCloud) {
  ransac::Params_ransac3d2d best_params;
  float best_score = 1e20;
  int best_leftoverPoints = 1e20;
  int best_numPrimitives = 0;

  float weight_primitives = 1.0;
  float weight_leftovers = 1.0;

  // 3D RANSAC parameters
  float probability = 0.01;
  std::vector<float> normalThresholds = {0.0, 0.5};
  std::vector<int> minPointsValues = {5, 10, 20};
  std::vector<float> epsilonValues = {0.01, 0.05, 0.1};
  std::vector<float> clusterEpsilonValues = {0.5, 1.0, 2.0};

  // 2D RANSAC parameters
  std::vector<int> maxIterationsValues = {100, 300};
  std::vector<int> minInliersValues = {4, 10};
  std::vector<float> toleranceValues = {0.01, 0.03, 0.05};
  std::vector<float> splitDistanceValues = {1.0, 1.5, 2.0};

  int total_combinations = normalThresholds.size() * minPointsValues.size() *
                           epsilonValues.size() * clusterEpsilonValues.size() *
                           maxIterationsValues.size() *
                           minInliersValues.size() * toleranceValues.size() *
                           splitDistanceValues.size();

  std::cout << "Starting parameter search for Ransac3D2D with "
            << total_combinations << " combinations..." << std::endl;
  std::cout << "Optimization targets: minimize primitives (weight="
            << weight_primitives
            << ") + minimize leftover points (weight=" << weight_leftovers
            << ")" << std::endl;

  int current_combination = 0;
  int progress_milestone = total_combinations / 10;  // 10% increments

  for (float normalThreshold : normalThresholds) {
    for (int minPoints : minPointsValues) {
      for (float epsilon : epsilonValues) {
        for (float clusterEpsilon : clusterEpsilonValues) {
          for (int maxIterations : maxIterationsValues) {
            for (int minInliers : minInliersValues) {
              for (float tolerance : toleranceValues) {
                for (float splitDistance : splitDistanceValues) {
                  current_combination++;
                  // Progress reporting every 10%
                  if (current_combination % progress_milestone == 0 ||
                      current_combination == total_combinations) {
                    int progress_percent =
                        (current_combination * 100) / total_combinations;
                    std::cout << "Progress: " << progress_percent << "% ("
                              << current_combination << "/"
                              << total_combinations
                              << " combinations completed)" << std::endl;
                  }

                  ransac::Params_ransac3d2d params;
                  params.normalThreshold = normalThreshold;
                  params.probability = probability;
                  params.minPoints = minPoints;
                  params.epsilon = epsilon;
                  params.clusterEpsilon = clusterEpsilon;
                  params.maxIterations = maxIterations;
                  params.minInliers = minInliers;
                  params.tolerance = tolerance;
                  params.splitDistanceThres = splitDistance;

                  ransac::Ransac3d2d ransac3d2d(pointCloud, params);
                  ransac3d2d.detect();

                  int numPrimitives = 0;
                  for (std::vector<ransac::Line3d> lines :
                       ransac3d2d.getLines3d()) {
                    numPrimitives += lines.size();
                  }
                  int leftoverPoints = ransac3d2d.getLeftoverIndices().size();

                  // Objective: maximize number of primitives, minimize
                  // leftover points
                  float score = weight_leftovers * leftoverPoints -
                                weight_primitives * numPrimitives;

                  if (score < best_score) {
                    best_score = score;
                    best_leftoverPoints = leftoverPoints;
                    best_numPrimitives = numPrimitives;
                    best_params = params;
                  }
                }
              }
            }
          }
        }
      }
    }
  }

  std::cout << "\n=== Parameter Search Results ===" << std::endl;
  std::cout << "Best parameters for Ransac3D2D: "
            << "normalThreshold: " << best_params.normalThreshold << ", "
            << "probability: " << best_params.probability << ", "
            << "minPoints: " << best_params.minPoints << ", "
            << "epsilon: " << best_params.epsilon << ", "
            << "clusterEpsilon: " << best_params.clusterEpsilon << ", "
            << "maxIterations: " << best_params.maxIterations << ", "
            << "minInliers: " << best_params.minInliers << ", "
            << "tolerance: " << best_params.tolerance << ", "
            << "splitDistanceThres: " << best_params.splitDistanceThres
            << std::endl;
  std::cout << "Number of primitives: " << best_numPrimitives << std::endl;
  std::cout << "Number of leftover points: " << best_leftoverPoints
            << std::endl;
  std::cout << "Combined score: " << best_score << std::endl;
  std::cout << "=== Search Complete ===" << std::endl;
}

void regionGrowing_paramSearch(easy3d::PointCloud* pointCloud) {
  regionGrowing::CylinderRegionGrowingParams best_params;
  float best_score = 1e20;
  int best_leftoverPoints = 1e20;
  int best_numPrimitives = 0;

  float weight_primitives = 1.0;
  float weight_leftovers = 1.0;

  std::vector<int> kValues = {12, 16, 20, 24, 28};
  std::vector<float> maxDistanceValues = {0.01, 0.02, 0.03, 0.04, 0.05};
  std::vector<float> maxAngleValues = {15, 20, 25, 30, 35};
  std::vector<int> minRegionSizeValues = {4, 10, 16, 20};
  float minRadius = 0.01;
  float maxRadius = 1.0;

  int total_combinations = kValues.size() * maxDistanceValues.size() *
                           maxAngleValues.size() * minRegionSizeValues.size();

  std::cout << "Starting parameter search for Region Growing with "
            << total_combinations << " combinations..." << std::endl;
  std::cout << "Optimization targets: minimize primitives (weight="
            << weight_primitives
            << ") + minimize leftover points (weight=" << weight_leftovers
            << ")" << std::endl;

  int current_combination = 0;
  int progress_milestone = total_combinations / 10;  // 10% increments

  for (int k : kValues) {
    for (float maxDistance : maxDistanceValues) {
      for (float maxAngle : maxAngleValues) {
        for (int minRegionSize : minRegionSizeValues) {
          current_combination++;

          // Progress reporting every 10%
          if (current_combination % progress_milestone == 0 ||
              current_combination == total_combinations) {
            int progress_percent =
                (current_combination * 100) / total_combinations;
            std::cout << "Progress: " << progress_percent << "% ("
                      << current_combination << "/" << total_combinations
                      << " combinations completed)" << std::endl;
          }

          regionGrowing::Point_set pointSet;
          if (!regionGrowing::makePointSet(pointCloud, pointSet, k)) {
            continue;
          }

          regionGrowing::CylinderRegionGrowingParams params;
          params.k = k;
          params.maxDistance = maxDistance;
          params.maxAngle = maxAngle;
          params.minRadius = minRadius;
          params.maxRadius = maxRadius;
          params.minRegionSize = minRegionSize;

          regionGrowing::CylinderRegionGrowing regionGrowing(pointSet, params);
          regionGrowing.detect();

          int numPrimitives = regionGrowing.getCylinders().size();
          int leftoverPoints = regionGrowing.getUnassignedIndices().size();

          // Objective: maximize number of primitives, minimize leftover
          // points
          float score = weight_leftovers * leftoverPoints -
                        weight_primitives * numPrimitives;

          if (score < best_score) {
            best_score = score;
            best_leftoverPoints = leftoverPoints;
            best_numPrimitives = numPrimitives;
            best_params = params;
          }
        }
      }
    }
  }

  std::cout << "\n=== Parameter Search Results ===" << std::endl;
  std::cout << "Best parameters for Region Growing: "
            << "k: " << best_params.k << ", "
            << "maxDistance: " << best_params.maxDistance << ", "
            << "maxAngle: " << best_params.maxAngle << ", "
            << "minRadius: " << best_params.minRadius << ", "
            << "maxRadius: " << best_params.maxRadius << ", "
            << "minRegionSize: " << best_params.minRegionSize << std::endl;
  std::cout << "Number of primitives: " << best_numPrimitives << std::endl;
  std::cout << "Number of leftover points: " << best_leftoverPoints
            << std::endl;
  std::cout << "Combined score: " << best_score << std::endl;
  std::cout << "=== Search Complete ===" << std::endl;
}

void energyMinimization_paramSearch(easy3d::PointCloud* pointCloud) {
  energyMinimization::EnergyMinimizationParams best_params;
  int num_outputs = 0;
  int best_preserved_edges = 0;

  // Graph construction parameters
  std::vector<int> kNeighborsValues = {8, 10, 12, 15};
  std::vector<float> edgeLengthThresValues = {1.5, 2.0, 2.5, 3.0};

  // Energy minimization parameters
  std::vector<float> lambdaValues = {0.05, 0.1, 0.15, 0.2, 0.25};
  std::vector<float> extendFactorValues = {1.5, 2.0, 2.5, 3.0};
  std::vector<float> inlierSearchRadiusValues = {0.8, 1.0, 1.2, 1.5};
  std::vector<float> minAngleValues = {5, 10, 15, 20};

  int total_combinations =
      kNeighborsValues.size() * edgeLengthThresValues.size() *
      lambdaValues.size() * extendFactorValues.size() *
      inlierSearchRadiusValues.size() * minAngleValues.size();

  std::cout << "Starting parameter search for Energy Minimization with "
            << total_combinations << " combinations..." << std::endl;

  int current_combination = 0;
  int progress_milestone = total_combinations / 10;  // 10% increments

  for (int kNeighbors : kNeighborsValues) {
    for (float edgeLengthThres : edgeLengthThresValues) {
      for (float lambda : lambdaValues) {
        for (float extendFactor : extendFactorValues) {
          for (float inlierSearchRadius : inlierSearchRadiusValues) {
            for (float minAngle : minAngleValues) {
              current_combination++;

              // Progress reporting every 10%
              if (current_combination % progress_milestone == 0 ||
                  current_combination == total_combinations) {
                int progress_percent =
                    (current_combination * 100) / total_combinations;
                std::cout << "Progress: " << progress_percent << "% ("
                          << current_combination << "/" << total_combinations
                          << " combinations completed)" << std::endl;
              }

              // Build graph
              graph::Graph graph(pointCloud, kNeighbors, edgeLengthThres);
              graph.buildMixGraph();
              auto mixGraph = graph.getMixGraph();
              graph.buildDualGraph(mixGraph);
              auto dualGraph = graph.getDualGraph();

              if (dualGraph->n_vertices() == 0 || dualGraph->n_edges() == 0) {
                continue;
              }

              energyMinimization::EnergyMinimizationParams params;
              params.lambda = lambda;
              params.extendFactor = extendFactor;
              params.inlierSearchRadius = inlierSearchRadius;
              params.minAngleInDegrees = minAngle;

              energyMinimization::EnergyMinimization energyMinimization(
                  dualGraph->n_vertices(), dualGraph->n_edges(), mixGraph,
                  pointCloud, params);
              energyMinimization.setDataTerm();
              energyMinimization.setSmoothnessTerm();
              energyMinimization.optimize();
              energyMinimization.getResults();

              easy3d::Graph* preservedGraph =
                  energyMinimization.getPreservedGraph();
              int preserved_edges = preservedGraph->n_edges();

              if (preserved_edges > best_preserved_edges) {
                best_preserved_edges = preserved_edges;
                best_params = params;
                num_outputs = preserved_edges;
              }
            }
          }
        }
      }
    }
  }

  std::cout << "\n=== Parameter Search Results ===" << std::endl;
  std::cout << "Best parameters for Energy Minimization: "
            << "lambda: " << best_params.lambda << ", "
            << "extendFactor: " << best_params.extendFactor << ", "
            << "inlierSearchRadius: " << best_params.inlierSearchRadius << ", "
            << "minAngleInDegrees: " << best_params.minAngleInDegrees
            << std::endl;
  std::cout << "Number of outputs (preserved edges): " << num_outputs
            << std::endl;
  std::cout << "=== Search Complete ===" << std::endl;
}