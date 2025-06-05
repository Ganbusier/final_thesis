#include <easy3d/core/point_cloud.h>
#include <easy3d/fileio/point_cloud_io.h>
#include <easy3d/renderer/drawable_lines.h>
#include <energyMinimization/energyMinimization.h>
#include <graph/graph.h>
#include <ransac/ransac3d.h>
#include <ransac/ransac3d2d.h>
#include <regionGrowing/cgalDefines_rg.h>
#include <regionGrowing/regionGrowing.h>

#include <chrono>
#include <iomanip>
#include <sstream>

#include "functions.h"

std::vector<easy3d::Drawable*> drawables;

// Global line color variable
static easy3d::vec4 current_line_color(0.984f, 0.333f, 0.490f,
                                       1.0f);  // Default pink #FB557D

// Function to get current line color
easy3d::vec4 get_current_line_color() { return current_line_color; }

// Function to set line color
void set_line_drawable_color(const easy3d::vec4& color) {
  current_line_color = color;
  // Update color of all existing line drawables
  for (auto& drawable : drawables) {
    auto lines_drawable = dynamic_cast<easy3d::LinesDrawable*>(drawable);
    if (lines_drawable) {
      lines_drawable->set_uniform_coloring(color);
    }
  }
}

// Helper function to get timestamp string
std::string get_timestamp_string() {
  auto now = std::chrono::system_clock::now();
  auto time = std::chrono::system_clock::to_time_t(now);
  std::stringstream ss;
  ss << std::put_time(std::localtime(&time), "%Y%m%d_%H%M%S");
  return ss.str();
}

// Helper function to create filename with timestamp
std::string create_timestamped_filename(const std::string& basePath,
                                        const std::string& prefix,
                                        const std::string& extension) {
  return basePath + "/" + prefix + "_" + get_timestamp_string() + extension;
}

bool run_EnergyMinimization(easy3d::Viewer* viewer, easy3d::Model* model,
                            const std::string& saveFilePath,
                            const easy3d::EnergyMinimizationParams& params) {
  if (!viewer || !model) return false;
  auto pointCloud = dynamic_cast<easy3d::PointCloud*>(model);

  graph::Graph graph(pointCloud, 10, 2.0f);
  graph.buildMixGraph();
  auto mixGraph = graph.getMixGraph();
  graph.buildDualGraph(mixGraph);
  auto dualGraph = graph.getDualGraph();

  energyMinimization::EnergyMinimizationParams em_params;
  em_params.lambda = params.lambda;
  em_params.extendFactor = params.extendFactor;
  em_params.inlierSearchRadius = params.inlierSearchRadius;
  em_params.minAngleInDegrees = params.minAngleInDegrees;

  energyMinimization::EnergyMinimization energyMinimization(
      dualGraph->n_vertices(), dualGraph->n_edges(), mixGraph, pointCloud,
      em_params);
  energyMinimization.setDataTerm();
  energyMinimization.setSmoothnessTerm();
  energyMinimization.optimize();
  energyMinimization.getResults();

  easy3d::Graph* preservedGraph = energyMinimization.getPreservedGraph();
  for (auto& drawable : drawables) {
    viewer->delete_drawable(drawable);
  }
  drawables.clear();
  
  if (preservedGraph->n_edges() > 0) {
    // Collect all vertices and edges into single buffers
    std::vector<easy3d::vec3> vertices;
    std::vector<unsigned int> indices;
    
    for (auto& edge : preservedGraph->edges()) {
      auto source = preservedGraph->source(edge);
      auto target = preservedGraph->target(edge);
      auto start = preservedGraph->position(source);
      auto end = preservedGraph->position(target);
      
      // Add vertices
      unsigned int start_idx = vertices.size();
      vertices.push_back(start);
      vertices.push_back(end);
      
      // Add line indices
      indices.push_back(start_idx);
      indices.push_back(start_idx + 1);
    }
    
    // Create single drawable for all preserved edges
    auto lineDrawable = new easy3d::LinesDrawable("preserved_edges");
    lineDrawable->set_impostor_type(easy3d::LinesDrawable::CYLINDER);
    lineDrawable->set_line_width(2.0f);
    lineDrawable->set_uniform_coloring(get_current_line_color());
    lineDrawable->update_vertex_buffer(vertices);
    lineDrawable->update_element_buffer(indices);
    viewer->add_drawable(lineDrawable);
    drawables.push_back(lineDrawable);
  }

  std::string preservedFilename =
      create_timestamped_filename(saveFilePath, "gco_preserved", ".ply");
  std::string removedFilename =
      create_timestamped_filename(saveFilePath, "gco_removed", ".ply");
  energyMinimization.saveResults(preservedFilename, removedFilename);

  return true;
}

bool run_RegionGrowing(easy3d::Viewer* viewer, easy3d::Model* model,
                       const std::string& saveFilePath,
                       const easy3d::RegionGrowingParams& params) {
  if (!viewer || !model) return false;
  auto pointCloud = dynamic_cast<easy3d::PointCloud*>(model);

  regionGrowing::Point_set pointSet;
  regionGrowing::makePointSet(pointCloud, pointSet, 16);
  regionGrowing::CylinderRegionGrowingParams rg_params;
  rg_params.k = params.k;
  rg_params.maxDistance = params.max_distance;
  rg_params.maxAngle = params.max_angle;
  rg_params.minRadius = params.min_radius;
  rg_params.maxRadius = params.max_radius;
  rg_params.minRegionSize = params.min_region_size;

  regionGrowing::CylinderRegionGrowing regionGrowing(pointSet, rg_params);
  regionGrowing.detect();

  std::vector<regionGrowing::Cylinder> cylinders = regionGrowing.getCylinders();

  if (cylinders.empty()) {
    LOG(INFO) << "No cylinders found";
    return true;
  }

  for (auto& drawable : drawables) {
    viewer->delete_drawable(drawable);
  }
  drawables.clear();

  for (size_t i = 0; i < cylinders.size(); ++i) {
    auto cylinder = cylinders[i];
    easy3d::vec3 start = easy3d::vec3(cylinder.start.x(), cylinder.start.y(),
                                      cylinder.start.z());
    easy3d::vec3 end =
        easy3d::vec3(cylinder.end.x(), cylinder.end.y(), cylinder.end.z());
    auto cylinderDrawable =
        new easy3d::LinesDrawable("cylinder" + std::to_string(i));
    cylinderDrawable->set_impostor_type(easy3d::LinesDrawable::CYLINDER);
    cylinderDrawable->set_line_width(cylinder.radius * 2.0f * 10.0f);
    cylinderDrawable->set_uniform_coloring(get_current_line_color());
    cylinderDrawable->update_vertex_buffer({start, end});
    cylinderDrawable->update_element_buffer({0, 1});
    viewer->add_drawable(cylinderDrawable);
    drawables.push_back(cylinderDrawable);
  }

  std::string cylinderInfosFilename =
      create_timestamped_filename(saveFilePath, "rg_cylinderInfos", ".csv");
  std::string unassignedPointsFilename =
      create_timestamped_filename(saveFilePath, "rg_unassignedPoints", ".ply");
  std::string lines3dFilename =
      create_timestamped_filename(saveFilePath, "rg_lines3d", ".ply");
  regionGrowing.saveCylinderInfos(cylinderInfosFilename);
  regionGrowing.saveUnassignedPoints(unassignedPointsFilename);
  regionGrowing.save3DLineSegments(lines3dFilename);

  return true;
}

bool run_Ransac3d(easy3d::Viewer* viewer, easy3d::Model* model,
                  const std::string& saveFilePath,
                  const easy3d::Ransac3dParams& params) {
  if (!viewer || !model) return false;
  auto pointCloud = dynamic_cast<easy3d::PointCloud*>(model);

  ransac::Params r3d_params;
  r3d_params.normalThreshold = params.normal_threshold;
  r3d_params.probability = params.probability;
  r3d_params.minPoints = params.min_points;
  r3d_params.epsilon = params.epsilon;
  r3d_params.clusterEpsilon = params.cluster_epsilon;
  r3d_params.minRadius = params.min_radius;
  r3d_params.maxRadius = params.max_radius;

  ransac::Ransac3d ransac3d(pointCloud, r3d_params);
  ransac3d.detect();

  std::vector<ransac::CylinderInfo> cylinderInfos = ransac3d.getCylinderInfos();

  if (cylinderInfos.empty()) {
    LOG(INFO) << "No cylinders found";
    return true;
  }

  for (auto& drawable : drawables) {
    viewer->delete_drawable(drawable);
  }
  drawables.clear();

  for (size_t i = 0; i < cylinderInfos.size(); ++i) {
    auto cylinderInfo = cylinderInfos[i];
    auto start = cylinderInfo.start;
    auto end = cylinderInfo.end;
    auto radius = cylinderInfo.radius;
    auto cylinderDrawable =
        new easy3d::LinesDrawable("cylinder" + std::to_string(i));
    cylinderDrawable->set_impostor_type(easy3d::LinesDrawable::CYLINDER);
    cylinderDrawable->set_line_width(radius * 2.0f);
    cylinderDrawable->set_uniform_coloring(get_current_line_color());
    cylinderDrawable->update_vertex_buffer({start, end});
    cylinderDrawable->update_element_buffer({0, 1});
    viewer->add_drawable(cylinderDrawable);
    drawables.push_back(cylinderDrawable);
  }

  std::string cylinderInfosFilename = create_timestamped_filename(
      saveFilePath, "ransac3d_cylinderInfos", ".csv");
  std::string leftoverPointsFilename = create_timestamped_filename(
      saveFilePath, "ransac3d_leftoverPoints", ".ply");
  std::string lines3dFilename =
      create_timestamped_filename(saveFilePath, "ransac3d_lines3d", ".ply");
  ransac3d.saveCylinderInfos(cylinderInfosFilename);
  ransac3d.saveLeftoverPoints(leftoverPointsFilename);
  ransac3d.save3DLineSegments(lines3dFilename);

  return true;
}

bool run_Ransac3d2d(easy3d::Viewer* viewer, easy3d::Model* model,
                    const std::string& saveFilePath,
                    const easy3d::Ransac3d2dParams& params) {
  if (!viewer || !model) return false;
  auto pointCloud = dynamic_cast<easy3d::PointCloud*>(model);

  ransac::Params_ransac3d2d r3d2d_params;
  r3d2d_params.normalThreshold = params.normal_threshold;
  r3d2d_params.probability = params.probability;
  r3d2d_params.minPoints = params.min_points;
  r3d2d_params.epsilon = params.epsilon;
  r3d2d_params.clusterEpsilon = params.cluster_epsilon;
  r3d2d_params.maxIterations = params.max_iterations;
  r3d2d_params.minInliers = params.min_inliers;
  r3d2d_params.tolerance = params.tolerance;
  r3d2d_params.splitDistanceThres = params.split_distance_threshold;

  ransac::Ransac3d2d ransac3d2d(pointCloud, r3d2d_params);
  ransac3d2d.detect();

  std::vector<std::vector<ransac::Line3d>> lines3d = ransac3d2d.getLines3d();
  if (lines3d.empty()) {
    LOG(INFO) << "No lines found";
    return true;
  }

  for (auto& drawable : drawables) {
    viewer->delete_drawable(drawable);
  }
  drawables.clear();

  // Collect all vertices and indices into single buffers
  std::vector<easy3d::vec3> vertices;
  std::vector<unsigned int> indices;
  
  for (size_t i = 0; i < lines3d.size(); ++i) {
    std::vector<ransac::Line3d> lines = lines3d[i];
    for (size_t j = 0; j < lines.size(); ++j) {
      ransac::Line3d line = lines[j];
      auto start = line.start;
      auto end = line.end;
      
      // Add vertices
      unsigned int start_idx = vertices.size();
      vertices.push_back(start);
      vertices.push_back(end);
      
      // Add line indices
      indices.push_back(start_idx);
      indices.push_back(start_idx + 1);
    }
  }
  
  if (!vertices.empty()) {
    // Create single drawable for all lines
    auto lineDrawable = new easy3d::LinesDrawable("ransac3d2d_lines");
    lineDrawable->set_impostor_type(easy3d::LinesDrawable::CYLINDER);
    lineDrawable->set_line_width(2.0f);
    lineDrawable->set_uniform_coloring(get_current_line_color());
    lineDrawable->update_vertex_buffer(vertices);
    lineDrawable->update_element_buffer(indices);
    viewer->add_drawable(lineDrawable);
    drawables.push_back(lineDrawable);
  }

  // Save results
  std::string linesFilename =
      create_timestamped_filename(saveFilePath, "ransac3d2d_lines", ".ply");
  std::string leftoverPointsFilename = create_timestamped_filename(
      saveFilePath, "ransac3d2d_leftoverPoints", ".ply");
  ransac3d2d.saveLines3d(linesFilename);
  ransac3d2d.saveLeftoverPoints(leftoverPointsFilename);

  return true;
}

void clear_algorithm_drawables() { drawables.clear(); }
