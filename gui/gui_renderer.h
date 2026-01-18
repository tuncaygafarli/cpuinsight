#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#pragma once
namespace gui {

    namespace renderer {

        bool init(const char* window_name, int32_t width, int32_t height); // creates window and imgui context if window creating was succesful
        bool window_closed();
        void cleanup();
        void render_frame();
        void new_frame();
        ImVec2 get_window_size();
    }

}