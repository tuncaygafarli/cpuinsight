#include <iostream>
#include <fstream>

#include "parser.h"
#include "../cpu/cpu.h"

#define EXPECT(EXPECTED_TOKEN_TYPE)                   \
    if (_current_token->type != EXPECTED_TOKEN_TYPE)   \
    {                                                 \
        std::cout << "Error : Token did not meet the expected type(Cause : " << _current_token->word << ")\n" ;               \
        exit(EXIT_FAILURE);                                       \
    }                                                             \
    
program_t&& parser_t::parse_program(const std::string &src)  {
    std::ifstream file(src);
    if (!file.is_open()) {
        std::cout << "File path " << src << " doesn't exist.\n";
        exit(EXIT_FAILURE);
    }
    std::string line_raw;
    while (std::getline(file, line_raw)) {
        tokenize_line_text(line_raw);
        _current_index = 0;
        _current_token = &_line_tokens[0];
        parse_instruction();
    }
    // resolve the unresolved instructions
    for (auto& branch_instruction_data : _unresolved_branch_instructions) {
        if(_label_map.find(branch_instruction_data.second) != _label_map.end()) {
            branch_instruction_data.first->_label_id = _label_map.at(branch_instruction_data.second);
        }else{
            std::cout << "Error : Unknown label identifier was found(Cause : " << branch_instruction_data.second << ")";
        }
    }
    for (auto&  jump_instruction_data : _unresolved_jump_instructions) {
        if(_label_map.find(jump_instruction_data.second) != _label_map.end()) {
            jump_instruction_data.first->_label_id = _label_map.at(jump_instruction_data.second);
        }else{
            std::cout << "Error : Unknown label identifier was found(Cause : " << jump_instruction_data.second << ")";
        }
    }
    file.close();
    return std::move(_program);
}

void parser_t::parse_instruction() {
    switch (_current_token->type) {
    case TOKEN_TYPE::LOAD_OPERATION:
    case TOKEN_TYPE::STORE_OPERATION:
        parse_mem_instruction();
        break;
    case TOKEN_TYPE::ALU_OPERATION_R:
    case TOKEN_TYPE::ALU_OPERATION_I:
        parse_alu_instruction();
        break;
    case TOKEN_TYPE::BRANCH_OPERATION:
        parse_branch_instruction();
        break;
    case TOKEN_TYPE::JUMP_OPERATION:
        parse_jump_instruction();
        break;
    case TOKEN_TYPE::LOAD_UPPER:
    case TOKEN_TYPE::AUIPC:
        parse_upperimm_instruction();
        break;
    case TOKEN_TYPE::LABEL:
        parse_label();
        break;
    default:
        break;
    }
}

// @call : current token is load or memory instruction
void parser_t::parse_mem_instruction() {
    token_t tmp = *_current_token;
    advance();
    // we should be register token
    EXPECT(TOKEN_TYPE::REGISTER);
    reg_id_t dest_reg_id = lookup_t::reg_id(_current_token->word);
    advance();
    // we should be comma
    EXPECT(TOKEN_TYPE::COMMA);

    advance();

    // we should be imm
    EXPECT(TOKEN_TYPE::IMMEDIATE);
    offset_t offset = std::stoi(_current_token->word);

    advance();
    // we should be left paranthesis
    EXPECT(TOKEN_TYPE::LPAREN);

    advance();
    // we should be register
    EXPECT(TOKEN_TYPE::REGISTER);
    reg_id_t src_reg_id = lookup_t::reg_id(_current_token->word);

    if(tmp.type == TOKEN_TYPE::LOAD_OPERATION) {
        load_instruction_t::LOAD_INSTRUCTION_TYPE type = lookup_t::load_type(tmp.word);
        _program.emplace_back(std::make_unique<load_instruction_t>(
            type,
            dest_reg_id,
            offset,
            src_reg_id
        ));
    } else if(tmp.type == TOKEN_TYPE::STORE_OPERATION) {
        store_instruction_t::STORE_INSTRUCTION_TYPE type = lookup_t::store_type(tmp.word);
        _program.emplace_back(std::make_unique<store_instruction_t>(
            type,
            dest_reg_id,
            offset,
            src_reg_id
        ));
    }
    advance();
    EXPECT(TOKEN_TYPE::RPAREN);
}

// @call : current token is alu operation
void parser_t::parse_alu_instruction() {
    token_t tmp = *_current_token;
    bool is_imm = false;
    alu_instruction_t::ALU_INSTRUCTION_TYPE type;
    advance();
    EXPECT(TOKEN_TYPE::REGISTER);
    reg_id_t dest_reg_id = lookup_t::reg_id(_current_token->word);
    advance();
    EXPECT(TOKEN_TYPE::COMMA);
    advance();
    EXPECT(TOKEN_TYPE::REGISTER);
    reg_id_t src1_reg_id = lookup_t::reg_id(_current_token->word);
    advance();
    EXPECT(TOKEN_TYPE::COMMA);
    advance();
    int64_t src2_val;
    // if our operation was imm
    if(lookup_t::alui_type(tmp.word) != alu_instruction_t::ALU_INSTRUCTION_TYPE::UNKNOWN) {
        type = lookup_t::alui_type(tmp.word);
        EXPECT(TOKEN_TYPE::IMMEDIATE);
        is_imm = true;
        if(_current_token->word[0] == '0' && _current_token->word[1] == 'x')
            src2_val = std::stoi(_current_token->word,nullptr,16);
        else
            src2_val = std::stoi(_current_token->word);
    } else if(lookup_t::alur_type(tmp.word) != alu_instruction_t::ALU_INSTRUCTION_TYPE::UNKNOWN) {
        type = lookup_t::alur_type(tmp.word);
        EXPECT(TOKEN_TYPE::REGISTER);
        src2_val = lookup_t::reg_id(_current_token->word);
    }
    advance();
    _program.emplace_back(
        std::make_unique<alu_instruction_t>(
            type,
            dest_reg_id,
            src1_reg_id,
            src2_val,
            is_imm
        )
    );
}

// @call : current token is branch instruction
void parser_t::parse_branch_instruction() {
    branch_instruction_t::BRANCH_INSTRUCTION_TYPE type = lookup_t::branch_type(_current_token->word);
    advance();
    EXPECT(TOKEN_TYPE::REGISTER);
    reg_id_t src1_id = lookup_t::reg_id(_current_token->word);
    advance();
    EXPECT(TOKEN_TYPE::COMMA);
    advance();
    EXPECT(TOKEN_TYPE::REGISTER);
    reg_id_t src2_id = lookup_t::reg_id(_current_token->word);
    advance();
    EXPECT(TOKEN_TYPE::COMMA);
    advance();
    EXPECT(TOKEN_TYPE::IDENTIFIER);
    label_id_t label_id = forward_label;
    if(_label_map.find(_current_token->word) != _label_map.end()) {
        label_id = _label_map.at(_current_token->word);
    }
    // build the instruction string
    std::string instr_str;
    instr_str.append(_line_tokens[0].word);
    instr_str.append(" ");
    for (size_t i = 1; i < _line_tokens.size();++i) {
        instr_str.append(_line_tokens[i].word);
    }
    auto branch_instruction = std::make_unique<branch_instruction_t>(
        type,
        src1_id,
        src2_id,
        label_id,
        instr_str,
        unique_branch_id()
    );
    auto branch_instruction_ptr = branch_instruction.get();
    _program.push_back(std::move(branch_instruction));
    // couldnt find the label identifier save it for later
    if(label_id == forward_label) {
        _unresolved_branch_instructions.emplace_back(branch_instruction_ptr,_current_token->word);
    }
}
// @call : current token is jump instruction
void parser_t::parse_jump_instruction() {
    label_id_t label_id = forward_label;
    jump_instruction_t::JUMP_INSTRUCTION_TYPE type = lookup_t::jump_type(_current_token->word);
    advance();
    EXPECT(TOKEN_TYPE::REGISTER);
    reg_id_t dest_reg = lookup_t::reg_id(_current_token->word);
    reg_id_t src1 = invalid_reg_id;
    advance();
    EXPECT(TOKEN_TYPE::COMMA);
    advance();
    if(type == jump_instruction_t::JUMP_INSTRUCTION_TYPE::JAL) {
        EXPECT(TOKEN_TYPE::IDENTIFIER);
        if(_label_map.find(_current_token->word) != _label_map.end()) {
            label_id = _label_map.at(_current_token->word);
        }
        // src1 and imm fields can be garbage values in this case
        auto jump_instruction = std::make_unique<jump_instruction_t>(
            type,
            dest_reg,
            src1,
            label_id,
            0
        );
        auto jump_instruction_ptr = jump_instruction.get();
        _program.push_back(
            std::move(jump_instruction)
        );
        // we werent able to find the label maybe we will next time
        if(label_id == forward_label)
            _unresolved_jump_instructions.emplace_back(jump_instruction_ptr,_current_token->word);
        return;
    }
    EXPECT(TOKEN_TYPE::REGISTER);
    src1 = lookup_t::reg_id(_current_token->word);
    advance();
    EXPECT(TOKEN_TYPE::COMMA);
    advance();
    EXPECT(TOKEN_TYPE::IMMEDIATE);
    int64_t imm;
    if(_current_token->word[0] == '0' && _current_token->word[1] == 'x')
        imm = std::stoi(_current_token->word,nullptr,16);
    else
        imm= std::stoi(_current_token->word);
    // here we dont care about the label_id field
    _program.emplace_back(
        std::make_unique<jump_instruction_t>(
            type,
            dest_reg,
            src1,
            label_id,
            imm
    ));
}

// @call : current token is lui or auipc
void parser_t::parse_upperimm_instruction() {
    token_t tmp = *_current_token;
    advance();
    EXPECT(TOKEN_TYPE::REGISTER);
    reg_id_t dest_reg = lookup_t::reg_id(_current_token->word);
    advance();
    EXPECT(TOKEN_TYPE::COMMA);
    advance();
    EXPECT(TOKEN_TYPE::IMMEDIATE);
    int64_t imm;
    if(_current_token->word[0] == '0' && _current_token->word[1] == 'x')

        imm = std::stoi(_current_token->word,nullptr,16);
    else
        imm= std::stoi(_current_token->word);
    if(tmp.word == "lui")
        _program.emplace_back(std::make_unique<load_upper_imm_instruction_t>(
            dest_reg,
            imm
        )
        );
    else if(tmp.word == "auipc")
        _program.emplace_back(std::make_unique<auipc_instruction_t>(
            dest_reg,
            imm
        )
        );
}

// @call : current token is label
void parser_t::parse_label() {
    label_id_t unique_id = unique_label_id();
    std::unique_ptr<label_instruction_t> label_instr = std::make_unique<label_instruction_t>(unique_id);
    // insert new entry to the label map
    _label_map.emplace(_current_token->word, unique_id);
    _program.push_back(std::move(label_instr));
    advance();
}
label_id_t parser_t::unique_label_id() {
    static label_id_t unique_id = forward_label + 1;
    return unique_id++;
}
branch_instruction_id_t parser_t::unique_branch_id() {
    static label_id_t unique_id = 0;
    return unique_id++;
}
void parser_t::advance() {

    if (_current_token->type != TOKEN_TYPE::NEW_LINE) {
		_current_index++;
		_current_token = &_line_tokens[_current_index];
	}
}

void parser_t::tokenize_line_text(const std::string& line_raw) {

    _line_tokens.clear();
    size_t comment_pos = line_raw.find('#');
    std::string line = (comment_pos != std::string::npos) ? line_raw.substr(0, comment_pos) : line_raw;
    if (line == line_raw) {
        comment_pos = line_raw.find(';');
        line = (comment_pos != std::string::npos) ? line_raw.substr(0, comment_pos) : line_raw;
    }
    size_t i = 0;
    while (i < line.size()) {
        char ch = line[i];

        // Skip whitespace
        if (std::isspace(static_cast<unsigned char>(ch))) {
            ++i;
            continue;
        }

        // Handle single-char _line_tokens
        if (ch == ',' || ch == '(' || ch == ')') {
            TOKEN_TYPE type;
            if (ch == ',') type = TOKEN_TYPE::COMMA;
            else if (ch == '(') type = TOKEN_TYPE::LPAREN;
            else type = TOKEN_TYPE::RPAREN;

            _line_tokens.emplace_back(std::string(1, ch), type);
            ++i;
            continue;
        }

        // Match [a-zA-Z0-9_.:-]+
        size_t start = i;
        while (i < line.size() && (std::isalnum(static_cast<unsigned char>(line[i])) || line[i] == '_' || line[i] == '.' || line[i] == '-' || line[i] == ':')) {
            ++i;
        }

        if (start == i)
            continue;

        std::string token = line.substr(start, i - start);

        // Handle label: `label:`
        if (!token.empty() && token.back() == ':') {
            _line_tokens.emplace_back(token.substr(0, token.length() - 1), TOKEN_TYPE::LABEL);
            continue;
        }

        if(lookup_t::load_type(token) != load_instruction_t::LOAD_INSTRUCTION_TYPE::UNKNOWN){
            _line_tokens.emplace_back(token, TOKEN_TYPE::LOAD_OPERATION);
        } else if(lookup_t::store_type(token) != store_instruction_t::STORE_INSTRUCTION_TYPE::UNKNOWN) {
            _line_tokens.emplace_back(token, TOKEN_TYPE::STORE_OPERATION);
        } else if(lookup_t::alui_type(token) != alu_instruction_t::ALU_INSTRUCTION_TYPE::UNKNOWN) {
            _line_tokens.emplace_back(token, TOKEN_TYPE::ALU_OPERATION_I);
        } else if(lookup_t::alur_type(token) != alu_instruction_t::ALU_INSTRUCTION_TYPE::UNKNOWN) {
            _line_tokens.emplace_back(token, TOKEN_TYPE::ALU_OPERATION_R);
        } else if(lookup_t::branch_type(token) != branch_instruction_t::BRANCH_INSTRUCTION_TYPE::UNKNOWN) {
            _line_tokens.emplace_back(token, TOKEN_TYPE::BRANCH_OPERATION);
        } else if(lookup_t::jump_type(token) != jump_instruction_t::JUMP_INSTRUCTION_TYPE::UNKNOWN) {
            _line_tokens.emplace_back(token, TOKEN_TYPE::JUMP_OPERATION);
        } else if(token == "lui") {
            _line_tokens.emplace_back(token, TOKEN_TYPE::LOAD_UPPER);
        } else if(token == "auipc") {
            _line_tokens.emplace_back(token, TOKEN_TYPE::AUIPC);
        } else if (lookup_t::reg_id(token) != invalid_reg_id) {
            _line_tokens.emplace_back(token, TOKEN_TYPE::REGISTER);
        }else if (lookup_t::is_imm(token)) {
            _line_tokens.emplace_back(token, TOKEN_TYPE::IMMEDIATE);
        }
        else {
            _line_tokens.emplace_back(token, TOKEN_TYPE::IDENTIFIER);
        }
    }
    _line_tokens.emplace_back(std::string(), TOKEN_TYPE::NEW_LINE);
}