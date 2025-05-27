#ifndef VIEWER_GUI_H
#define VIEWER_GUI_H

#include <easy3d/viewer/viewer.h>

struct ImGuiContext;

namespace easy3d {
    class ViewerGUI : public Viewer {
    public:
        explicit ViewerGUI(
            const std::string& title = "Geomatics Thesis GUI",
            int samples = 4,
            int gl_major = 3,
            int gl_minor = 2,
            bool full_screen = false,
            bool resizable = true,
            int depth_bits = 24,
            int stencil_bits = 8,
            int width = 1200,
            int height = 800
        );

        ~ViewerGUI() override;

    protected:
        // ImGui initialization
        void init() override;

        // Draw GUI before rendering
        void pre_draw() override;

        // Draw GUI after rendering
        void post_draw() override;

        void post_resize(int w, int h) override;

        // Event handling
        bool callback_event_cursor_pos(double x, double y) override;
        bool callback_event_mouse_button(int button, int action, int modifiers) override;
        bool callback_event_keyboard(int key, int action, int modifiers) override;
        bool callback_event_character(unsigned int codepoint) override;
        bool callback_event_scroll(double dx, double dy) override;

        // Menu functions
        void draw_menu_file();
        void draw_menu_algorithms();
        void draw_menu_view();
        
        // Additional panels
        void draw_model_info_panel();
        
        // Utility functions for file dialogs
        std::string open_folder_dialog();

        // Algorithm panels
        void draw_energy_minimization_panel();
        void draw_region_growing_panel();
        void draw_ransac3d_panel();
        void draw_ransac3d2d_panel();

        // Color update functions
        void update_point_color();
        void update_line_color();

        // Utility functions
        float pixel_ratio();
        float widget_scaling() { return dpi_scaling() / pixel_ratio(); }
        void reload_font(int font_size = 16);

    private:
        // ImGui context
        static ImGuiContext* context_;

        // GUI state
        float menu_height_;
        bool show_energy_minimization_panel_;
        bool show_region_growing_panel_;
        bool show_ransac3d_panel_;
        bool show_ransac3d2d_panel_;
        bool show_model_info_panel_;

        // Color settings
        easy3d::vec3 point_color_;
        easy3d::vec3 line_color_;

        // Algorithm parameters
        struct EnergyMinimizationParams {
            int k_neighbors = 10;
            float radius = 2.0f;
        } em_params_;

        struct RegionGrowingParams {
            int k = 16;
            float max_distance = 0.1f;
            float max_angle = 25.0f;
            float min_radius = 0.01f;
            float max_radius = 1.0f;
            int min_region_size = 4;
        } rg_params_;

        struct Ransac3dParams {
            float normal_threshold = 0.9f;
            float probability = 0.01f;
            int min_points = 10;
            float epsilon = 0.05f;
            float cluster_epsilon = 1.0f;
            float min_radius = 0.01f;
            float max_radius = 1.0f;
        } r3d_params_;

        struct Ransac3d2dParams {
            float normal_threshold = 0.9f;
            float probability = 0.01f;
            int min_points = 10;
            float epsilon = 0.05f;
            float cluster_epsilon = 1.0f;
            int max_iterations = 200;
            int min_inliers = 4;
            float tolerance = 0.1f;
            float split_distance_threshold = 2.0f;
        } r3d2d_params_;

        // Output folder path
        std::string output_folder_path_;
    };
}

#endif // VIEWER_GUI_H 