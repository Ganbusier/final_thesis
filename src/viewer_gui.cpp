#include "viewer_gui.h"
#include "functions.h"

#include <easy3d/util/file_system.h>
#include <easy3d/core/point_cloud.h>
#include <easy3d/renderer/text_renderer.h>
#include <easy3d/renderer/camera.h>
#include <easy3d/renderer/renderer.h>
#include <easy3d/renderer/drawable_points.h>

#include <3rd_party/glfw/include/GLFW/glfw3.h>
#include <3rd_party/imgui/misc/fonts/imgui_fonts_droid_sans.h>
#include <3rd_party/imgui/imgui.h>
#include <3rd_party/imgui/backends/imgui_impl_glfw.h>
#include <3rd_party/imgui/backends/imgui_impl_opengl3.h>

#ifdef _WIN32
#include <windows.h>
#include <commdlg.h>
#include <shlobj.h>
#include <objbase.h>
#endif

namespace easy3d {

ImGuiContext* ViewerGUI::context_ = nullptr;

ViewerGUI::ViewerGUI(
    const std::string& title,
    int samples,
    int gl_major,
    int gl_minor,
    bool full_screen,
    bool resizable,
    int depth_bits,
    int stencil_bits,
    int width,
    int height
) : Viewer(title, samples, gl_major, gl_minor, full_screen, resizable, depth_bits, stencil_bits, width, height),
    menu_height_(0.0f),
    show_energy_minimization_panel_(false),
    show_region_growing_panel_(false),
    show_ransac3d_panel_(false),
    show_ransac3d2d_panel_(false),
    show_model_info_panel_(false),
    point_color_(0.0f, 0.663f, 0.988f),  // Default blue #00A9FC
    line_color_(0.984f, 0.333f, 0.490f),   // Default pink #FB557D
    output_folder_path_("./")
{
#if defined(_WIN32) && defined(_MSC_VER)
    glfwInit();
#endif
    camera()->setUpVector(vec3(0, 1, 0));
    camera()->setViewDirection(vec3(0, 0, -1));
    camera_->showEntireScene();
}

ViewerGUI::~ViewerGUI() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext(context_);
}

void ViewerGUI::init() {
    Viewer::init();

    if (!context_) {
        IMGUI_CHECKVERSION();
        context_ = ImGui::CreateContext();

        const char* glsl_version = "#version 150";
        ImGui_ImplGlfw_InitForOpenGL(window_, true);
        ImGui_ImplOpenGL3_Init(glsl_version);
        
        ImGuiIO& io = ImGui::GetIO();
        io.WantCaptureKeyboard = true;
        io.WantTextInput = true;
        io.IniFilename = nullptr;
        
        ImGui::StyleColorsDark();
        ImGuiStyle& style = ImGui::GetStyle();
        style.FrameRounding = 5.0f;

        reload_font();
    }
}

float ViewerGUI::pixel_ratio() {
    int fw = framebuffer_width();
    int vw = width();
    return static_cast<float>(fw) / static_cast<float>(vw);
}

void ViewerGUI::reload_font(int font_size) {
    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->Clear();
    io.Fonts->AddFontFromMemoryCompressedTTF(droid_sans_compressed_data, droid_sans_compressed_size, 
                                             font_size * dpi_scaling());
    io.Fonts->Build();
    ImGui_ImplOpenGL3_DestroyFontsTexture();
}

void ViewerGUI::pre_draw() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    Viewer::pre_draw();
}

void ViewerGUI::post_draw() {
    // Main menu bar
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 8));
    if (ImGui::BeginMainMenuBar()) {
        draw_menu_file();
        draw_menu_algorithms();
        draw_menu_view();
        menu_height_ = ImGui::GetWindowHeight();
        ImGui::EndMainMenuBar();
    }
    ImGui::PopStyleVar();

    // Additional panels
    if (show_model_info_panel_) {
        draw_model_info_panel();
    }
    
    // Algorithm panels
    if (show_energy_minimization_panel_) {
        draw_energy_minimization_panel();
    }
    if (show_region_growing_panel_) {
        draw_region_growing_panel();
    }
    if (show_ransac3d_panel_) {
        draw_ransac3d_panel();
    }
    if (show_ransac3d2d_panel_) {
        draw_ransac3d2d_panel();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Draw Easy3D logo
    auto texter = texter_;
    texter_ = nullptr;
    Viewer::post_draw();
    texter_ = texter;

    if (texter_) {
        const float font_size = 15.0f;
        const float offset_x = (width() - texter_->string_width("Easy3D", font_size) - 20.0f) * dpi_scaling();
        const float offset_y = (20.0f + menu_height_) * dpi_scaling();
        texter_->draw("Easy3D", offset_x, offset_y, font_size, 0);
    }
}

void ViewerGUI::post_resize(int w, int h) {
    Viewer::post_resize(w, h);
}

bool ViewerGUI::callback_event_cursor_pos(double x, double y) {
    if (ImGui::GetIO().WantCaptureMouse)
        return true;
    return Viewer::callback_event_cursor_pos(x, y);
}

bool ViewerGUI::callback_event_mouse_button(int button, int action, int modifiers) {
    if (ImGui::GetIO().WantCaptureMouse)
        return true;
    return Viewer::callback_event_mouse_button(button, action, modifiers);
}

bool ViewerGUI::callback_event_keyboard(int key, int action, int modifiers) {
    if (ImGui::GetIO().WantCaptureKeyboard)
        return true;
    
    // Handle custom keyboard shortcuts
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        // Delete current model (Delete key)
        if (key == GLFW_KEY_DELETE && modifiers == 0) {
            if (current_model()) {
                clear_algorithm_drawables();
                delete_model(current_model());
                LOG(INFO) << "Current model deleted via keyboard shortcut";
                return true;
            }
        }
        // Clear all models (Ctrl+Delete)
        else if (key == GLFW_KEY_DELETE && (modifiers & GLFW_MOD_CONTROL)) {
            if (!models().empty()) {
                clear_algorithm_drawables();
                clear_scene();
                LOG(INFO) << "All models cleared via keyboard shortcut";
                return true;
            }
        }
    }
    
    return Viewer::callback_event_keyboard(key, action, modifiers);
}

bool ViewerGUI::callback_event_character(unsigned int codepoint) {
    if (ImGui::GetIO().WantCaptureKeyboard)
        return true;
    return Viewer::callback_event_character(codepoint);
}

bool ViewerGUI::callback_event_scroll(double dx, double dy) {
    if (ImGui::GetIO().WantCaptureMouse)
        return true;
    return Viewer::callback_event_scroll(dx, dy);
}

void ViewerGUI::draw_menu_file() {
    if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("Open", "Ctrl+O")) {
            open();
        }
        if (ImGui::MenuItem("Save As...", "Ctrl+S")) {
            save();
        }
        ImGui::Separator();
        
        // Model management
        if (ImGui::MenuItem("Delete Current Model", "Delete", false, current_model() != nullptr)) {
            if (current_model()) {
                clear_algorithm_drawables();
                delete_model(current_model());
                LOG(INFO) << "Current model deleted";
            }
        }
        if (ImGui::MenuItem("Clear All Models", "Ctrl+Delete", false, !models().empty())) {
            clear_algorithm_drawables();
            clear_scene();
            LOG(INFO) << "All models cleared";
        }
        ImGui::Separator();
        
        // Output folder selection
        ImGui::Text("Output Folder:");
        char buffer[256];
        strncpy(buffer, output_folder_path_.c_str(), sizeof(buffer));
        buffer[sizeof(buffer) - 1] = '\0';
        
        // Make the input text field wider
        ImGui::PushItemWidth(200.0f);
        if (ImGui::InputText("##output_folder", buffer, sizeof(buffer))) {
            output_folder_path_ = std::string(buffer);
        }
        ImGui::PopItemWidth();
        
        ImGui::SameLine();
        if (ImGui::Button("Browse...")) {
            std::string selected_folder = open_folder_dialog();
            if (!selected_folder.empty()) {
                output_folder_path_ = selected_folder;
                LOG(INFO) << "Output folder set to: " << output_folder_path_;
            }
        }
        
        ImGui::Separator();
        if (ImGui::MenuItem("Quit", "Alt+F4")) {
            glfwSetWindowShouldClose(window_, GLFW_TRUE);
        }
        ImGui::EndMenu();
    }
}

void ViewerGUI::draw_menu_algorithms() {
    if (ImGui::BeginMenu("Algorithms")) {
        if (ImGui::MenuItem("Energy Minimization", nullptr, show_energy_minimization_panel_)) {
            show_energy_minimization_panel_ = !show_energy_minimization_panel_;
        }
        if (ImGui::MenuItem("Region Growing", nullptr, show_region_growing_panel_)) {
            show_region_growing_panel_ = !show_region_growing_panel_;
        }
        if (ImGui::MenuItem("RANSAC 3D", nullptr, show_ransac3d_panel_)) {
            show_ransac3d_panel_ = !show_ransac3d_panel_;
        }
        if (ImGui::MenuItem("RANSAC 3D2D", nullptr, show_ransac3d2d_panel_)) {
            show_ransac3d2d_panel_ = !show_ransac3d2d_panel_;
        }
        ImGui::EndMenu();
    }
}

void ViewerGUI::draw_menu_view() {
    if (ImGui::BeginMenu("View")) {
        if (ImGui::MenuItem("Snapshot", nullptr)) {
            snapshot();
        }
        ImGui::Separator();
        
        // Panel toggles
        if (ImGui::MenuItem("Model Info", nullptr, show_model_info_panel_)) {
            show_model_info_panel_ = !show_model_info_panel_;
        }
        
        ImGui::Separator();
        ImGui::ColorEdit3("Background Color", (float*)background_color_, ImGuiColorEditFlags_NoInputs);
        
        ImGui::Separator();
        // Point color setting
        if (ImGui::ColorEdit3("Point Color", (float*)&point_color_, ImGuiColorEditFlags_NoInputs)) {
            update_point_color();
        }
        
        // Line color setting
        if (ImGui::ColorEdit3("Line Color", (float*)&line_color_, ImGuiColorEditFlags_NoInputs)) {
            update_line_color();
        }
        
        ImGui::EndMenu();
    }
}

void ViewerGUI::draw_energy_minimization_panel() {
    ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Energy Minimization", &show_energy_minimization_panel_)) {
        ImGui::Text("Parameters for computing the dual graph:");
        ImGui::SliderInt("K Neighbors", &em_params_.k_neighbors, 5, 50);
        ImGui::SliderFloat("Edge Length Threshold", &em_params_.edgeLengthThres, 0.1f, 10.0f);
        ImGui::Separator();
        ImGui::Text("Parameters for energy minimization:");
        ImGui::SliderFloat("Lambda", &em_params_.lambda, 0.0f, 1.0f);
        ImGui::SliderFloat("Extend Factor", &em_params_.extendFactor, 1.0f, 10.0f);
        ImGui::SliderFloat("Inlier Search Radius", &em_params_.inlierSearchRadius, 0.1f, 10.0f);
        ImGui::SliderFloat("Min Angle", &em_params_.minAngleInDegrees, 0.0f, 90.0f);


        ImGui::Separator();
        if (ImGui::Button("Run Energy Minimization")) {
            if (current_model()) {
                run_EnergyMinimization(this, current_model(), output_folder_path_, em_params_);
            } else {
                LOG(WARNING) << "No model loaded";
            }
        }
    }
    ImGui::End();
}

void ViewerGUI::draw_region_growing_panel() {
    ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Region Growing", &show_region_growing_panel_)) {
        ImGui::Text("Parameters:");
        ImGui::SliderInt("K", &rg_params_.k, 8, 32);
        ImGui::SliderFloat("Max Distance", &rg_params_.max_distance, 0.01f, 1.0f);
        ImGui::SliderFloat("Max Angle", &rg_params_.max_angle, 5.0f, 45.0f);
        ImGui::SliderFloat("Min Radius", &rg_params_.min_radius, 0.001f, 0.1f);
        ImGui::SliderFloat("Max Radius", &rg_params_.max_radius, 0.1f, 5.0f);
        ImGui::SliderInt("Min Region Size", &rg_params_.min_region_size, 2, 20);
        
        ImGui::Separator();
        if (ImGui::Button("Run Region Growing")) {
            if (current_model()) {
                run_RegionGrowing(this, current_model(), output_folder_path_, rg_params_);
            } else {
                LOG(WARNING) << "No model loaded";
            }
        }
    }
    ImGui::End();
}

void ViewerGUI::draw_ransac3d_panel() {
    ImGui::SetNextWindowSize(ImVec2(300, 350), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("RANSAC 3D", &show_ransac3d_panel_)) {
        ImGui::Text("Parameters:");
        ImGui::SliderFloat("Normal Threshold", &r3d_params_.normal_threshold, 0.1f, 1.0f);
        ImGui::SliderFloat("Probability", &r3d_params_.probability, 0.001f, 0.1f, "%.3f");
        ImGui::SliderInt("Min Points", &r3d_params_.min_points, 5, 50);
        ImGui::SliderFloat("Epsilon", &r3d_params_.epsilon, 0.01f, 0.2f);
        ImGui::SliderFloat("Cluster Epsilon", &r3d_params_.cluster_epsilon, 0.1f, 5.0f);
        ImGui::SliderFloat("Min Radius", &r3d_params_.min_radius, 0.001f, 0.1f);
        ImGui::SliderFloat("Max Radius", &r3d_params_.max_radius, 0.1f, 5.0f);
        
        ImGui::Separator();
        if (ImGui::Button("Run RANSAC 3D")) {
            if (current_model()) {
                run_Ransac3d(this, current_model(), output_folder_path_, r3d_params_);
            } else {
                LOG(WARNING) << "No model loaded";
            }
        }
    }
    ImGui::End();
}

void ViewerGUI::draw_ransac3d2d_panel() {
    ImGui::SetNextWindowSize(ImVec2(300, 400), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("RANSAC 3D2D", &show_ransac3d2d_panel_)) {
        ImGui::Text("3D Parameters:");
        ImGui::SliderFloat("Normal Threshold", &r3d2d_params_.normal_threshold, 0.1f, 1.0f);
        ImGui::SliderFloat("Probability", &r3d2d_params_.probability, 0.001f, 0.1f, "%.3f");
        ImGui::SliderInt("Min Points", &r3d2d_params_.min_points, 5, 50);
        ImGui::SliderFloat("Epsilon", &r3d2d_params_.epsilon, 0.01f, 0.2f);
        ImGui::SliderFloat("Cluster Epsilon", &r3d2d_params_.cluster_epsilon, 0.1f, 5.0f);
        
        ImGui::Separator();
        ImGui::Text("2D Parameters:");
        ImGui::SliderInt("Max Iterations", &r3d2d_params_.max_iterations, 50, 500);
        ImGui::SliderInt("Min Inliers", &r3d2d_params_.min_inliers, 2, 20);
        ImGui::SliderFloat("Tolerance", &r3d2d_params_.tolerance, 0.01f, 1.0f);
        ImGui::SliderFloat("Split Distance", &r3d2d_params_.split_distance_threshold, 0.5f, 10.0f);
        
        ImGui::Separator();
        if (ImGui::Button("Run RANSAC 3D2D")) {
            if (current_model()) {
                run_Ransac3d2d(this, current_model(), output_folder_path_, r3d2d_params_);
            } else {
                LOG(WARNING) << "No model loaded";
            }
        }
    }
    ImGui::End();
}

void ViewerGUI::draw_model_info_panel() {
    ImGui::SetNextWindowSize(ImVec2(350, 250), ImGuiCond_FirstUseEver);
    if (ImGui::Begin("Model Information", &show_model_info_panel_)) {
        const auto& model_list = models();
        
        ImGui::Text("Loaded Models: %zu", model_list.size());
        ImGui::Separator();
        
        if (model_list.empty()) {
            ImGui::Text("No models loaded");
            ImGui::Text("Use File -> Open to load a point cloud");
        } else {
            // Current model info
            auto current = current_model();
            if (current) {
                ImGui::Text("Current Model:");
                ImGui::Indent();
                ImGui::Text("Name: %s", easy3d::file_system::simple_name(current->name()).c_str());
                
                // Check if it's a point cloud
                auto point_cloud = dynamic_cast<easy3d::PointCloud*>(current);
                if (point_cloud) {
                    ImGui::Text("Type: Point Cloud");
                    ImGui::Text("Vertices: %u", point_cloud->n_vertices());
                    
                    // Show bounding box
                    auto box = point_cloud->bounding_box();
                    if (box.is_valid()) {
                        auto min_pt = box.min_point();
                        auto max_pt = box.max_point();
                        auto size = max_pt - min_pt;
                        ImGui::Text("Bounding Box:");
                        ImGui::Text("  Size: (%.2f, %.2f, %.2f)", size.x, size.y, size.z);
                        ImGui::Text("  Min: (%.2f, %.2f, %.2f)", min_pt.x, min_pt.y, min_pt.z);
                        ImGui::Text("  Max: (%.2f, %.2f, %.2f)", max_pt.x, max_pt.y, max_pt.z);
                    }
                }
                ImGui::Unindent();
                
                ImGui::Separator();
                
                // Action buttons
                if (ImGui::Button("Delete Current Model")) {
                    clear_algorithm_drawables();
                    delete_model(current);
                    LOG(INFO) << "Current model deleted from info panel";
                }
                ImGui::SameLine();
                if (ImGui::Button("Fit Screen")) {
                    fit_screen(current);
                }
            }
            
            ImGui::Separator();
            
            // All models list
            if (model_list.size() > 1) {
                ImGui::Text("All Models:");
                for (size_t i = 0; i < model_list.size(); ++i) {
                    auto model = model_list[i];
                    bool is_current = (model == current);
                    
                    ImGui::PushID(static_cast<int>(i));
                    
                    // Display model name with current indicator
                    std::string display_name = easy3d::file_system::simple_name(model->name());
                    if (is_current) {
                        display_name = "* " + display_name + " (current)";
                    }
                    ImGui::Text("%s", display_name.c_str());
                    
                    // Context menu for each model
                    if (ImGui::BeginPopupContextItem()) {
                        if (ImGui::MenuItem("Delete")) {
                            clear_algorithm_drawables();
                            delete_model(model);
                            LOG(INFO) << "Model deleted from context menu";
                        }
                        if (ImGui::MenuItem("Fit Screen")) {
                            fit_screen(model);
                        }
                        ImGui::EndPopup();
                    }
                    ImGui::PopID();
                }
            }
            
            ImGui::Separator();
            if (ImGui::Button("Clear All Models")) {
                clear_algorithm_drawables();
                clear_scene();
                LOG(INFO) << "All models cleared from info panel";
            }
        }
        
        // Output folder settings
        ImGui::Separator();
        ImGui::Text("Output Settings:");
        ImGui::Text("Current Output Folder:");
        ImGui::TextWrapped("%s", output_folder_path_.c_str());
        
        if (ImGui::Button("Change Output Folder")) {
            std::string selected_folder = open_folder_dialog();
            if (!selected_folder.empty()) {
                output_folder_path_ = selected_folder;
                LOG(INFO) << "Output folder changed to: " << output_folder_path_;
            }
        }
    }
    ImGui::End();
}

std::string ViewerGUI::open_folder_dialog() {
#ifdef _WIN32
    // Initialize COM
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (FAILED(hr)) {
        LOG(ERROR) << "Failed to initialize COM";
        return "";
    }

    std::string selected_folder;
    
    // Create the File Open Dialog object
    IFileOpenDialog *pFileOpen = nullptr;
    hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, 
                         IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

    if (SUCCEEDED(hr)) {
        // Set options to pick folders
        DWORD dwOptions;
        hr = pFileOpen->GetOptions(&dwOptions);
        if (SUCCEEDED(hr)) {
            hr = pFileOpen->SetOptions(dwOptions | FOS_PICKFOLDERS);
        }

        // Show the Open dialog box
        if (SUCCEEDED(hr)) {
            hr = pFileOpen->Show(NULL);

            // Get the file name from the dialog box
            if (SUCCEEDED(hr)) {
                IShellItem *pItem;
                hr = pFileOpen->GetResult(&pItem);
                if (SUCCEEDED(hr)) {
                    PWSTR pszFilePath;
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                    // Display the file name to the user
                    if (SUCCEEDED(hr)) {
                        // Convert wide string to regular string
                        int size_needed = WideCharToMultiByte(CP_UTF8, 0, pszFilePath, -1, NULL, 0, NULL, NULL);
                        if (size_needed > 0) {
                            std::string str(size_needed - 1, 0);
                            WideCharToMultiByte(CP_UTF8, 0, pszFilePath, -1, &str[0], size_needed, NULL, NULL);
                            selected_folder = str;
                        }
                        CoTaskMemFree(pszFilePath);
                    }
                    pItem->Release();
                }
            }
        }
        pFileOpen->Release();
    }
    
    CoUninitialize();
    return selected_folder;
#else
    // For non-Windows platforms, return empty string for now
    LOG(WARNING) << "Folder dialog not implemented for this platform";
    return "";
#endif
}

void ViewerGUI::update_point_color() {
    // Update point cloud color of current model
    auto current = current_model();
    if (current) {
        auto point_cloud = dynamic_cast<easy3d::PointCloud*>(current);
        if (point_cloud) {
            auto vertices_drawable = point_cloud->renderer()->get_points_drawable("vertices");
            if (vertices_drawable) {
                easy3d::vec4 color(point_color_.x, point_color_.y, point_color_.z, 1.0f);
                vertices_drawable->set_uniform_coloring(color);
                update();
                LOG(INFO) << "Point color updated to (" << point_color_.x << ", " << point_color_.y << ", " << point_color_.z << ")";
            }
        }
    }
}

void ViewerGUI::update_line_color() {
    // Update color of algorithm-generated lines
    easy3d::vec4 color(line_color_.x, line_color_.y, line_color_.z, 1.0f);
    set_line_drawable_color(color);
    update();
    LOG(INFO) << "Line color updated to (" << line_color_.x << ", " << line_color_.y << ", " << line_color_.z << ")";
}

} // namespace easy3d 