#include "parser/parser.h"
#include "cpu/cpu.h"

#include "gui/gui_renderer.h"
#include "gui/gui_reg_file.h"
#include "gui/gui_assembly.h"
#include <thread>
#include <chrono>

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

    gui::reg_file_element_t gui_reg_file(&cpu.get_reg_file());
    gui::assembly_element_t gui_assembly(std::move(parse_result.second),&cpu.get_pc());

    if (!window)
        return 1;

    while (!glfwWindowShouldClose(window)) {
        if (!cpu.halt())
            cpu.execute();

        gui::renderer::new_frame();
        gui_assembly.draw(); 
        gui_reg_file.draw(); 
        gui::renderer::render_frame(window);
    }

    gui::renderer::cleanup(window);
}
