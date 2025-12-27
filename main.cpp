#include <iostream>
#include <fstream>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>

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
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "BranchPredictor", sf::Style::Fullscreen);

    int selection_index = 0;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--log") {
            if (i + 1 < argc) {
                log_destination = argv[i + 1];
                i++;
            }
            else {
                std::cerr << "Error: --log requires a destination (cout or a filename).\n";
                return 1;
            }
        }
        else if (arg == "--gshare") {
            predictor_type = CPU::PREDICTOR_TYPE::GSHARE;
        }
        else if (arg == "--GAg") {
            predictor_type = CPU::PREDICTOR_TYPE::GAg;
        }
        else if (arg == "--PAg") {
            predictor_type = CPU::PREDICTOR_TYPE::PAg;
        }
        else if (arg == "--simple") {
            predictor_type = CPU::PREDICTOR_TYPE::SIMPLE;
        }
        else if (arg.rfind("--", 0) == 0) { // Checks if the argument starts with '--'
            std::cerr << "Warning: Unknown option: " << arg << ". Ignoring.\n";
        }
        else {
            if (input_file.empty()) {
                input_file = arg;
            }
            else {
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
    }
    else if (log_destination != "none") {
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
    bool cpu_halted = cpu.halt();
    bool autorun = false;

    sf::Clock autorun_timer;
    float autorun_delay = 0.5f;

    while (window.isOpen())
    {
        float delta_time = autorun_timer.restart().asSeconds();
        static float accumulator = 0.f;

        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Down) {
                    selection_index++;
                    if (selection_index >= gui_render.instruction_elements.size()) {
                        selection_index = 0;
                    }
                    gui_render.set_selection(selection_index);
                    gui_render.ensure_visible(selection_index);
                }

                if (event.key.code == sf::Keyboard::Up) {
                    selection_index--;
                    if (selection_index < 0) {
                        selection_index = gui_render.instruction_elements.size() - 1;
                    }
                    gui_render.set_selection(selection_index);
                    gui_render.ensure_visible(selection_index);
                }

                if (event.key.code == sf::Keyboard::Right) {
                    gui_render.scroll_registers(gui_render.visible_height);
                }

                if (event.key.code == sf::Keyboard::Left) {
                    gui_render.scroll_registers(-gui_render.visible_height);
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

                if (event.key.code == sf::Keyboard::V) {
                    autorun = !autorun;
                    accumulator = 0.f;
                }

                if (event.key.code == sf::Keyboard::R) {
                        cpu.reset();

                        gui_render.update_registers(cpu);
                        selection_index = cpu.get_pc();
                        gui_render.set_selection(selection_index);
                        gui_render.ensure_visible(selection_index);
                }

                if (event.key.code == sf::Keyboard::LShift) {
                    autorun_delay += 1.f;
                }

                if (event.key.code == sf::Keyboard::LControl) {
                    autorun_delay = std::max(0.1f, autorun_delay - 0.1f);
                }
            }
        }

        if (autorun && !cpu_halted) {
            accumulator += delta_time;

            while (accumulator >= autorun_delay && !cpu_halted) {
                accumulator -= autorun_delay;

                cpu.execute();
                cpu_halted = cpu.halt();

                gui_render.update_registers(cpu);
                selection_index = cpu.get_pc();
                gui_render.set_selection(selection_index);
                gui_render.ensure_visible(selection_index);
            }
        }

        window.clear(sf::Color(30, 30, 35, 230));
        gui_render.draw_gui(window, cpu);
        window.display();
    }

    return 0;
}