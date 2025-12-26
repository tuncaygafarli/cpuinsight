#pragma once

#include <vector>
#include <utility>

#include "token.h"
#include "lookup.h"
#include "../cpu/cpu.h"
struct cli_args_t {
    std::string input_file;
    CPU::PREDICTOR_TYPE predictor;
    std::string log_dest;
    bool enable_gui;
    bool valid;
};
class parser_t {
public:
    parser_t() = default;
    std::pair<cpu_program_t,std::vector<std::string>>  parse_program(const std::string& src);
    cli_args_t  parse_cli(int argc, char** argv);
private:
    void        parse_instruction();
    void        parse_pseudo_instruction();
    void        parse_mem_instruction();
    void        parse_alu_instruction();
    void        parse_branch_instruction();
    void        parse_jump_instruction();
    void        parse_upperimm_instruction();
    void        parse_label();
    void        tokenize_line_text(const std::string &line_raw);
    void        advance();
    label_id_t  unique_label_id();
    branch_instruction_id_t unique_branch_id();
private:
    token_t*                                                             _current_token = nullptr;
    size_t                                                               _current_index = 0;
    std::unordered_map<std::string, label_id_t>                          _label_map;
    std::vector<std::pair<branch_instruction_t*,std::string>>            _unresolved_branch_instructions;// the instructions whose labels are yet to be found
    std::vector<std::pair<jump_instruction_t*,std::string>>              _unresolved_jump_instructions;// the instructions whose labels are yet to be found
    std::vector<token_t>                                                 _line_tokens;
    cpu_program_t                                                            _program;
    size_t                                                               _line_number = 0;
    size_t                                                               _column = 0;
};
