#ifndef RANSAC3D2D_H
#define RANSAC3D2D_H

#include <easy3d/core/point_cloud.h>
#include <easy3d/fileio/point_cloud_io.h>

#include <random>

#include "cgalDefines_ransac.h"


namespace ransac {
struct Line2d {
  double a;
  double b;
  double c;
  easy3d::vec2 start;
  easy3d::vec2 end;
  std::vector<int> inlierIndices;
};

struct Line3d {
  easy3d::vec3 start;
  easy3d::vec3 end;
  float length;
  std::vector<int> inlierIndices;
};

struct Params_ransac3d2d {
  // Ransac 3d params
  FT normalThreshold = 0.9;
  FT probability = 0.01;
  FT minPoints = 20;
  FT epsilon = 0.05;
  FT clusterEpsilon = 1.0;
  // Ransac 2d params
  size_t maxIterations = 200;
  size_t minInliers = 4;
  double tolerance = 0.1;
  double splitDistanceThres = 2.0;
};

class Ransac3d2d {
 public:
  Ransac3d2d(easy3d::PointCloud* pointCloud, const Params_ransac3d2d& params);
  void detect();
  const std::vector<std::vector<Line3d>>& getLines3d() const { return m_lines3d; }
  const std::vector<int>& getLeftoverIndices() const {
    return m_leftoverIndices;
  }
  const std::vector<easy3d::vec3>& getLeftoverPoints() const {
    return m_leftoverPoints;
  }
  void saveLines3d(const std::string& filename);
  void saveLeftoverPoints(const std::string& filename);

 private:
  easy3d::PointCloud* m_pointCloud;
  Pwn_vector m_pwnVector;
  Params_ransac3d2d m_params;
  Efficient_ransac m_ransac3d;
  std::vector<Plane> m_planes;
  std::vector<std::vector<Line2d>> m_lines2d;
  std::vector<std::vector<Line3d>> m_lines3d;
  std::vector<int> m_leftoverIndices;
  std::vector<easy3d::vec3> m_leftoverPoints;
  std::mt19937 rng{std::random_device{}()};

  void detectPlanes();
  void detectLines2d();
  void lines2dToLines3d();
  void pointCloudToPwnVector();
  void storeLeftoverIndices();
  void storeLeftoverPoints();

  Line2d computeLineModel(const easy3d::vec2& p1, const easy3d::vec2& p2);
  double distanceToLine(const easy3d::vec2& p, const Line2d& line);
  void refineLineWithPCA(Line2d& line,
                         const std::vector<easy3d::vec2>& inliers);
  std::vector<Line2d> splitLine(
      const Line2d& line,
      const std::vector<std::pair<easy3d::vec2, int>>& inlierData,
      double distanceThreshold = 5.0);
};

}  // namespace ransac

#endif  // RANSAC3D2D_H
