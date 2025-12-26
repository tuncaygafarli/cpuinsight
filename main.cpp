#include "parser/parser.h"
#include "cpu/cpu.h"

#include "gui/gui_renderer.h"
#include "gui/gui_elements.h"

#include <chrono>
#include <iostream>
int main(int argc, char** argv) {

    parser_t parser;
    auto cli_info  = parser.parse_cli(argc, argv);

    if (!cli_info.valid)
        return 1;

    CPU cpu(cli_info.predictor);
    auto parse_result = parser.parse_program(cli_info.input_file);

    cpu.load_program(std::move(parse_result.first));

    gui::renderer::init("BranchPredict", 800, 600);
    GLFWwindow* window = gui::renderer::get_window();

    if (!window)
        return 1;

	float cpu_speed = 0;
    CPU_EXECUTION action = CPU_EXECUTION::STOP;
    bool follow_pc = false;
	double accumulator = 0.0;
    auto last_time = std::chrono::high_resolution_clock::now();
    const double MAX_IPS = 60.0; 


    while (!glfwWindowShouldClose(window)) {
        gui::renderer::new_frame();
        ImGui::SetNextWindowPos({ 0,0 });
        ImGui::SetNextWindowSize(gui::renderer::get_window_size());
        if (ImGui::Begin("CPU Visual", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse)) {

            gui::draw(cpu,parse_result.second,follow_pc,action,cpu_speed);
            auto now = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> delta = now - last_time;
            last_time = now;

            switch (action) {
            case CPU_EXECUTION::RUN:
                accumulator += cpu_speed * MAX_IPS * delta.count();
                while (accumulator >= 1.0 && !cpu.endofprogram()) {
                    cpu.execute();
                    accumulator -= 1.0;
                }
                break;
            case CPU_EXECUTION::STEP:
                if (!cpu.endofprogram())
                    cpu.execute();
                action = CPU_EXECUTION::STOP;
                break;
            case CPU_EXECUTION::RESET:
                cpu.reset();
                break;
            case CPU_EXECUTION::STOP:
            default:
                break;

            }
            ImGui::End();
        }
        gui::renderer::render_frame(window);
    }

    gui::renderer::cleanup(window);
}
