#include <easy3d/util/initializer.h>
#include <easy3d/renderer/camera.h>
#include <easy3d/core/model.h>
#include <easy3d/renderer/drawable_points.h>
#include <easy3d/renderer/renderer.h>
#include "viewer_gui.h"

int main(int argc, char** argv) {
    // Initialize Easy3D
    easy3d::initialize(true);
    LOG(INFO) << "Easy3D initialized";

    // Create GUI viewer
    easy3d::ViewerGUI viewer("Geomatics Thesis - GUI Version");
    viewer.set_background_color(easy3d::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    viewer.camera()->setType(easy3d::Camera::ORTHOGRAPHIC);
    viewer.camera()->setZNearCoefficient(1e-10f);
    viewer.camera()->setZClippingCoefficient(1e10f);

    // Load default model if provided
    if (argc >= 2) {
        std::string inputFilePath = argv[1];
        easy3d::Model* model = viewer.add_model(inputFilePath, true);
        if (model) {
            auto drawable = model->renderer()->get_points_drawable("vertices");
            if (drawable) {
                drawable->set_uniform_coloring(easy3d::vec4(0.0f, 0.6627f, 0.9882f, 1.0f));
                drawable->set_impostor_type(easy3d::PointsDrawable::PLAIN);
                drawable->set_point_size(3.0f);
            }
        }
    }

    viewer.fit_screen();
    return viewer.run();
} 