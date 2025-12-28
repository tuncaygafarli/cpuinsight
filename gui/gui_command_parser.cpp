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

GUICommandParser::GUICommandParser(GUIRender& gui_render, CPU& cpu, parser_t& parser) : gui_render(gui_render), cpu(cpu), parser(parser) {}

void GUICommandParser::parse_and_execute(const std::string& command_line) {
    add_command_history(command_line);

    std::istringstream iss(command_line);
    std::string cmd;
    iss >> cmd;

    std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

    if (std::find(commands.begin(), commands.end(), cmd) == commands.end()) {
        std::ostringstream err_oss;
        err_oss << "Unknown command \'" << cmd << "\'." << "\nPlease run \"help\" to see available command list." << "\n";
        gui_render.output_message = err_oss.str();
    }

    // help command
    if (cmd == "help") {
        std::ostringstream oss;
        oss << "=== CPUInsight Command List ===" << "\n";
        oss << "help            | Shows this message" << "\n";
        oss << "load [filename] | Loads RISC-V Assembly file" << "\n";
        oss << "mode            | Sets current predictor mode" << "\n";
        oss << "stats           | Shows statistics for executed instructions" << "\n";
        oss << "keybindings     | Shows current keybinding list" << "\n";
        oss << "run             | Runs the loaded RISC-V Assembly file" << "\n";
        oss << "stop            | Stops the current execution" << "\n";
        oss << "exit            | Terminates the program" << "\n";

        gui_render.output_message = oss.str();
    }

    // load command
    if (cmd == "load") {
        std::string filename;
        iss >> filename;

        if (filename.empty()) {
            std::cout << "ERROR: No filename!" << std::endl;
            gui_render.output_message = "ERROR: No filename provided!";
            gui_render.set_show_output(true);
            return;
        }

        gui_render.set_parser_err(false);
        gui_render.instruction_codes.clear();
        cpu.reset();

        try {
            cpu.load_program(parser.parse_program(filename, gui_render));

            if (gui_render.get_parser_err()) {
                if (gui_render.output_message.empty()) {
                    gui_render.output_message = "Couldn't find or parse the requested file.";
                }
                gui_render.set_show_output(true);
                return;
            }

            gui_render.update_instructions(cpu);
            gui_render.update_registers(cpu);
            gui_render.set_show_output(true);
            gui_render.output_message = "Successfully loaded: " + filename;

        }
        catch (const std::exception& e) {
            gui_render.output_message = "Error loading file: " + std::string(e.what());
            gui_render.set_show_output(true);
        }
    }

    if (cmd == "mode") {
        std::string mode;
        iss >> mode;
        std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

        if (mode == "simple") {
            cpu.set_branch_predictor(CPU::PREDICTOR_TYPE::SIMPLE);
            gui_render.output_message = "Predictor set to: Simple";
        }
        else if (mode == "gag") {
            cpu.set_branch_predictor(CPU::PREDICTOR_TYPE::GAg);
            gui_render.output_message = "Predictor set to: GAg";
        }
        else if (mode == "pag") {
            cpu.set_branch_predictor(CPU::PREDICTOR_TYPE::PAg);
            gui_render.output_message = "Predictor set to: PAg";
        }
        else if (mode == "gshare") {
            cpu.set_branch_predictor(CPU::PREDICTOR_TYPE::GSHARE);
            gui_render.output_message = "Predictor set to: GShare";
        }
        else {
            gui_render.output_message = "Unknown predictor. Available: simple, gag, pag, gshare";
        }
    }

    // stats command
   if (cmd == "stats") {
       std::ostringstream oss;
       oss << "=== CPU Statistics ===" << "\n";
       oss << "Instructions: " << gui_render.instruction_codes.size() << "\n";
       oss << "Correct predictions: " << cpu.get_correct_predictions() << "\n";
       oss << "Prediction accuracy: " << cpu.get_accuracy() * 100 << "%" << "\n";
       oss << "Cycles: " << cpu.get_cycles() << "\n";

       gui_render.output_message = oss.str();
    }

   // keybindings command
   if (cmd == "keybindings") {
       std::ostringstream oss;
       oss << "=== CPUInsight Keybinding List ===" << "\n";
       oss << "ESC       | Switches between CLI and NAVIGATION modes" << "\n";
       oss << "ArrowDown | Scrolls down in INSTRUCTION section" << "\n";
       oss << "SpaceBar  | Executes only one instruction" << "\n";
       oss << "V         | Enables / disables automatic execution" << "\n";
       oss << "R         | Resets the process" << "\n";
       oss << "LShift    | Increases auto CPU execution delay" << "\n";
       oss << "LControl  | Decreases auto CPU execution delay" << "\n";

       gui_render.output_message = oss.str();
   }

   // run command
   if (cmd == "run") {
       if (gui_render.instruction_elements.size() == 0) {
           gui_render.output_message = "Couldn't execute the instructions, please load a file first.";
           return;
       }
       gui_render.set_autorun(true);
       gui_render.set_accumulator(0.f);

       gui_render.output_message = "Running the instructions...";
   }

   // stop command
   if (cmd == "stop") {
       gui_render.set_autorun(false);
       gui_render.set_accumulator(0.f);

       gui_render.output_message = "Stopped the process.";
   }

   // exit command
   if (cmd == "exit") {
       exit_requested = true;
   }
}