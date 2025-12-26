#include "gui_renderer.h"
#include <iostream>

namespace gui {
    namespace renderer {
        static GLFWwindow* window = nullptr;
        void init(const char* window_name, int32_t width, int32_t height) {

            if (!glfwInit()) {
                std::cerr << "Error : Failed to initialize GLFW\n";
                return;
            }

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            window = glfwCreateWindow(width, height, window_name, nullptr, nullptr);
            if (!window) {
                std::cerr << "Failed to create GLFW window\n";
                glfwTerminate();
                return;
            }
            glfwMakeContextCurrent(window);
            glfwSwapInterval(1);
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();

            ImGui::StyleColorsDark(); 

            ImGui_ImplGlfw_InitForOpenGL(window, true);
            ImGui_ImplOpenGL3_Init("#version 330"); 
        }
        void cleanup(GLFWwindow* window) {

            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();

            glfwDestroyWindow(window);
            glfwTerminate();
        }
        void new_frame() {
            glfwPollEvents();
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
        }
        void render_frame(GLFWwindow* window) {
            ImGui::Render();
            int display_w = 0;
            int display_h = 0;
            glfwGetFramebufferSize(window, &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);

        }

        GLFWwindow* get_window() {
            return window;
        }
        ImVec2 get_window_size() {
            int display_w, display_h;
            glfwGetFramebufferSize(window, &display_w, &display_h);
            return { static_cast<float>(display_w),static_cast<float>(display_h) };
        }
    } // namespace renderer

} // namespace gui