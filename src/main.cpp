#include <easy3d/renderer/camera.h>
#include <easy3d/renderer/drawable_points.h>
#include <easy3d/renderer/renderer.h>
#include <easy3d/util/initializer.h>

#include "functions.h"


std::string g_outputFolderPath;

bool EnergyMinimization(easy3d::Viewer* viewer, easy3d::Model* model);
bool RegionGrowing(easy3d::Viewer* viewer, easy3d::Model* model);
bool Ransac3d(easy3d::Viewer* viewer, easy3d::Model* model);

int main(int argc, char** argv) {
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0]
              << " <input_file_path> <output_folder_path>" << std::endl;
    return -1;
  }

  std::string inputFilePath = argv[1];
  g_outputFolderPath = argv[2];

  easy3d::initialize(true);
  LOG(INFO) << "Easy3D initialized";

  easy3d::Viewer viewer("Geomatics Thesis");
  viewer.set_background_color(easy3d::vec4(1.0f, 1.0f, 1.0f, 1.0f));
  viewer.camera()->setType(easy3d::Camera::ORTHOGRAPHIC);
  easy3d::Model* model = viewer.add_model(inputFilePath, true);

  auto drawable = model->renderer()->get_points_drawable("vertices");
  drawable->set_uniform_coloring(easy3d::vec4(0.0f, 0.6627f, 0.9882f, 1.0f));
  drawable->set_impostor_type(easy3d::PointsDrawable::PLAIN);
  drawable->set_point_size(3.0f);

  viewer.bind(EnergyMinimization, model, easy3d::Viewer::KEY_E,
              easy3d::Viewer::MODIF_SHIFT);
  viewer.bind(RegionGrowing, model, easy3d::Viewer::KEY_G,
              easy3d::Viewer::MODIF_SHIFT);
  viewer.bind(Ransac3d, model, easy3d::Viewer::KEY_R,
              easy3d::Viewer::MODIF_SHIFT);


  viewer.fit_screen();
  viewer.run();

  return 0;
}

bool EnergyMinimization(easy3d::Viewer* viewer, easy3d::Model* model) {
  std::string savefilepath = g_outputFolderPath;
  run_EnergyMinimization(viewer, model, savefilepath);
  return true;
}

bool RegionGrowing(easy3d::Viewer* viewer, easy3d::Model* model) {
  std::string savefilepath = g_outputFolderPath;
  run_RegionGrowing(viewer, model, savefilepath);
  return true;
}

bool Ransac3d(easy3d::Viewer* viewer, easy3d::Model* model) {
  std::string savefilepath = g_outputFolderPath;
  run_Ransac3d(viewer, model, savefilepath);
  return true;
}
