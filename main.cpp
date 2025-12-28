#include <iostream>
#include <fstream>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>

#include <SFML/Graphics.hpp>
#include "gui/gui_render.h"
#include "gui/gui_command_parser.h"

#include "parser/parser.h"
#include "cpu/cpu.h"

int main(int argc, char** argv) {
    // GUI TEST
    GUIRender gui_render;

    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "CPUInsight", sf::Style::Fullscreen);

    parser_t parser;
    CPU cpu(CPU::PREDICTOR_TYPE::GSHARE);
    GUICommandParser gc_parser(gui_render, cpu, parser);

    gui_render.init(cpu);
    gui_render.run(window, cpu, gc_parser);

    return 0;
}