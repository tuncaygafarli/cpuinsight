#pragma once

#include <vector>
#include <utility>

#include "token.h"
#include "lookup.h"
class parser_t {
public:
    parser_t() = default;
    program_t&&  parse_program(const std::string& src);
private:
    void        parse_instruction();
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
    program_t                                                            _program;
};
