#include <iostream>
#include <fstream>
#include <string>
#include <iostream>

#include <SFML/Graphics.hpp>
#include "gui/gui_render.h"

#include "parser/parser.h"
#include "cpu/cpu.h"

int main(int argc, char** argv) {
    std::string input_file;
    std::string log_destination = "none"; 
    CPU::PREDICTOR_TYPE predictor_type = CPU::PREDICTOR_TYPE::GSHARE; 

    // GUI TEST
    GUIRender gui_render;
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "BranchPredictor");

    int selection_index = 0;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--log") {
            if (i + 1 < argc) {
                log_destination = argv[i + 1];
                i++; 
            } else {
                std::cerr << "Error: --log requires a destination (cout or a filename).\n";
                return 1;
            }
        } else if (arg == "--gshare") {
            predictor_type = CPU::PREDICTOR_TYPE::GSHARE;
        } else if (arg == "--GAg") {
            predictor_type = CPU::PREDICTOR_TYPE::GAg;
        } else if (arg == "--PAg") {
            predictor_type = CPU::PREDICTOR_TYPE::PAg;
        } else if (arg == "--simple") {
            predictor_type = CPU::PREDICTOR_TYPE::SIMPLE;
        } else if (arg.rfind("--", 0) == 0) { // Checks if the argument starts with '--'
            std::cerr << "Warning: Unknown option: " << arg << ". Ignoring.\n";
        } else {
            if (input_file.empty()) {
                input_file = arg;
            } else {
                std::cerr << "Warning: Multiple input files found. Using first one: " << input_file << ".\n";
            }
        }
    }

    if (input_file.empty()) {
        std::cerr << "Usage: " << argv[0] << " <input.s> [--log cout | --log <filename>] [--gshare | --GAg | --PAg | --simple]\n";
        return 1;
    }

    std::ofstream log_file;
    std::ostream* log_stream = nullptr;  
	if (log_destination == "cout") {
        log_stream = &std::cout;
    } else if (log_destination != "none") {
        log_file.open(log_destination);
        if (!log_file.is_open()) {
            std::cerr << "Error: Could not open log file '" << log_destination << "'\n";
            return 1;
        }
        log_stream = &log_file;
    } 

    parser_t parser;
    CPU cpu(predictor_type); 

    cpu.load_program(parser.parse_program(input_file, gui_render)); 
    gui_render.init(cpu);
    bool cpu_halted = false;

    /*
    while (!cpu.halt()) {
        cpu.execute();
    }
    */
    
    
    if (log_stream) {
        cpu.log(*log_stream);  
    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                selection_index++;
                if (selection_index >= gui_render.instruction_elements.size()) {
                    selection_index = 0;
                }
                gui_render.set_selection(selection_index);
                gui_render.ensure_visible(selection_index);
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                selection_index--;

                if (selection_index < 0) {
                    selection_index = gui_render.instruction_elements.size() - 1;
                }

                gui_render.set_selection(selection_index);
                gui_render.ensure_visible(selection_index);
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::PageDown)) {
                gui_render.scroll(100.f);  // Scroll down
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::PageUp)) {
                gui_render.scroll(-100.f); // Scroll up
            }

            if (event.key.code == sf::Keyboard::Space) {
                if (!cpu_halted) {
                    cpu.execute();
                    cpu_halted = cpu.halt();

                    gui_render.update_registers(cpu);

                    selection_index = cpu.get_pc();
                    gui_render.set_selection(selection_index);
                    gui_render.ensure_visible(selection_index);
                }
            }
        }

        window.clear(sf::Color(30, 30, 35, 230));
        gui_render.draw_gui(window, cpu);
        window.display();
    }

    return 0;
}