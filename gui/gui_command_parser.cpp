#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>

#include "gui_command_parser.h"
#include "gui_render.h"
#include "../cpu/cpu.h"
#include "../parser/parser.h"

parser_t parser;

GUICommandParser::GUICommandParser(GUIRender& gui_render, CPU& cpu, parser_t& parser) : gui_render(gui_render), cpu(cpu), parser(parser) {
    std::cout << "All classes initialized!" << std::endl;
}

void GUICommandParser::parse_and_execute(const std::string& command_line) {
    std::istringstream iss(command_line);
    std::string cmd;
    iss >> cmd;

    std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

    if (cmd == "load") {
        std::string filename;
        iss >> filename;

        std::cout << "Filename: " << filename << std::endl;

        if (filename.empty()) {
            std::cout << "ERROR: No filename!" << std::endl;
            return;
        }

        gui_render.instruction_codes.clear();
        cpu.reset();
        cpu.load_program(parser.parse_program(filename, gui_render));
        gui_render.update_instructions(cpu);
        gui_render.update_registers(cpu);
    }

   if (cmd == "stats") {
        std::cout << "=== CPU Statistics ===" << std::endl;
        std::cout << "Instructions executed: " << gui_render.instruction_codes.size() << std::endl;
        std::cout << "Branch predictions: " << cpu.get_correct_predictions() << std::endl;
        std::cout << "Prediction accuracy: " << cpu.get_accuracy() * 100 << "%" << std::endl;
        std::cout << "Cycles: " << cpu.get_cycles() << std::endl;
    }
}