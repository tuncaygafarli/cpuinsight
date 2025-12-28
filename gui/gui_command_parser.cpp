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

    if (cmd == "help") {
        std::ostringstream oss;
        oss << "=== CPUInsight Command List ===" << "\n";
        oss << "help : Shows this message" << "\n";
        oss << "load [filename] : Loads RISC-V Assembly file" << "\n";
        oss << "stats : Shows statistics for executed instructions" << "\n";
        oss << "leybindings : Shows current keybinding list" << "\n";
        oss << "exit : Terminates the program" << "\n";

        gui_render.output_message = oss.str();
    }

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

        gui_render.set_show_output(true);
        gui_render.output_message = "Successfully loaded: " + filename;
    }

   if (cmd == "stats") {
       std::ostringstream oss;
       oss << "=== CPU Statistics ===" << "\n";
       oss << "Instructions: " << gui_render.instruction_codes.size() << "\n";
       oss << "Correct predictions: " << cpu.get_correct_predictions() << "\n";
       oss << "Prediction accuracy: " << cpu.get_accuracy() * 100 << "%" << "\n";
       oss << "Cycles: " << cpu.get_cycles() << "\n";

       gui_render.output_message = oss.str();
    }


   if (cmd == "keybindings") {
       std::ostringstream oss;
       oss << "=== CPUInsight Keybinding List ===" << "\n";
       oss << "ArrowDown : Scrolls down in INSTRUCTION section" << "\n";
       oss << "SpaceBar : Executes only one instruction" << "\n";
       oss << "V : Enables / disables automatic execution" << "\n";
       oss << "R : Resets the process" << "\n";
       oss << "LShift : Increases auto CPU execution delay" << "\n";
       oss << "LControl : Decreases auto CPU execution delay" << "\n";

       gui_render.output_message = oss.str();
   }

   if (cmd == "exit") {
       exit_requested = true;
   }
}