#include "functions.h"

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <input_file_path>" << std::endl;
    return -1;
  }

  std::string inputFilePath = argv[1];
  auto pointCloud = easy3d::PointCloudIO::load(inputFilePath);
  if (pointCloud->empty()) {
    LOG(ERROR) << "Failed to load point cloud from " << inputFilePath;
    return -1;
  }


  run_EnergyMinimization(pointCloud, "../data/gco");

  return 0;
}