#pragma once
#include <string>
#include <vector>
#include <functional>
#include <map>

#include "../cpu/cpu.h"
#include "gui_render.h"
#include "../parser/parser.h"

class GUICommandParser {
public:
    GUICommandParser(GUIRender& gui_render, CPU& cpu, parser_t& parser);
    void parse_and_execute(const std::string& command_line);

private:
    CPU& cpu;
    GUIRender& gui_render;
    parser_t& parser;
    std::vector<std::string> commands = { "load", "run" };
};