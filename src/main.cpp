#include "functions.h"

bool EnergyMinimization(easy3d::Viewer* viewer, easy3d::Model* model);

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <input_file_path>" << std::endl;
    return -1;
  }

  std::string inputFilePath = argv[1];

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

  viewer.bind(EnergyMinimization, model, easy3d::Viewer::KEY_E, easy3d::Viewer::MODIF_CTRL);

  viewer.fit_screen();
  viewer.run();

  return 0;
}

bool EnergyMinimization(easy3d::Viewer* viewer, easy3d::Model* model) {
  std::string savefilepath = "output/result.txt";
  run_EnergyMinimization(viewer, model, savefilepath);
  return true;
}
