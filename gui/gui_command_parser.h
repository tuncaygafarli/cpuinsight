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

    bool should_exit() const { return exit_requested; }

private:
    CPU& cpu;
    GUIRender& gui_render;
    parser_t& parser;

    bool exit_requested = false;

    const std::vector<std::string> commands = { "load", "exit" };
};