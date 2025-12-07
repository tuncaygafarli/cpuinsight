#include "lookup.h"

const std::unordered_map<std::string,reg_id_t> 
lookup_t::_registers = {
	{"zero",0},{"ra",1},{"sp",2},{"gp",3},
	{"tp",4},{"t0",5},{"t1",6},{"t2",7},
	{"s0",8},{"s1",9},{"a0",10},
	{"a1",11}, {"a2",12}, {"a3",13}, {"a4",14},
	{"a5",15}, {"a6",16}, {"a7",17}, {"s2",18},
	{"s3",19}, {"s4",20}, {"s5",21}, {"s6",22},
	{"s7",23},{"s8",24},{"s9",25}, {"s10",26},
	{"s11",27},{"t3",28}, {"t4",29}, {"t5",30},{"t6",31},{"fp",8},
	{"x0",0},{"x1",1},{"x2",2},{"x3",3},
	{"x4",4},{"x5",5},{"x6",6},{"x7",7},
	{"x8",8},{"x9",9},{"x10",10},
	{"x11",11}, {"x12",12}, {"x13",13}, {"x14",14},
	{"x15",15}, {"x16",16}, {"x17",17}, {"x18",18},
	{"x19",19}, {"x20",20}, {"x21",21}, {"x22",22},
	{"x23",23},{"x24",24},{"x25",25}, {"x26",26},
	{"x27",27},{"x28",28}, {"x29",29}, {"x30",30},{"x31",31}
};
const std::unordered_map<std::string, load_instruction_t::LOAD_INSTRUCTION_TYPE> 
lookup_t::_load_instructions = {
    {"lw", load_instruction_t::LOAD_INSTRUCTION_TYPE::LW},
    {"lhu", load_instruction_t::LOAD_INSTRUCTION_TYPE::LHU},
    {"lh", load_instruction_t::LOAD_INSTRUCTION_TYPE::LH},
    {"lbu", load_instruction_t::LOAD_INSTRUCTION_TYPE::LBU},
    {"lb", load_instruction_t::LOAD_INSTRUCTION_TYPE::LB}
};
const std::unordered_map<std::string,store_instruction_t::STORE_INSTRUCTION_TYPE>     
lookup_t::_store_instructions = {
    {"sb", store_instruction_t::STORE_INSTRUCTION_TYPE::SB},
    {"sw", store_instruction_t::STORE_INSTRUCTION_TYPE::SW},
    {"sh", store_instruction_t::STORE_INSTRUCTION_TYPE::SH}
};
const std::unordered_map<std::string, alu_instruction_t::ALU_INSTRUCTION_TYPE>
lookup_t::_alui_instructions = {
    {"addi", alu_instruction_t::ALU_INSTRUCTION_TYPE::ADD},
    {"slli", alu_instruction_t::ALU_INSTRUCTION_TYPE::SLL},
    {"slti", alu_instruction_t::ALU_INSTRUCTION_TYPE::SLT},
    {"sltiu", alu_instruction_t::ALU_INSTRUCTION_TYPE::SLTU},
    {"xori", alu_instruction_t::ALU_INSTRUCTION_TYPE::XOR},
    {"srli", alu_instruction_t::ALU_INSTRUCTION_TYPE::SRL},
    {"srai", alu_instruction_t::ALU_INSTRUCTION_TYPE::SRA},
    {"ori",  alu_instruction_t::ALU_INSTRUCTION_TYPE::OR},
    {"andi",alu_instruction_t::ALU_INSTRUCTION_TYPE::AND}
};

const std::unordered_map<std::string,alu_instruction_t::ALU_INSTRUCTION_TYPE>         
lookup_t::_alur_instructions = {
    {"add", alu_instruction_t::ALU_INSTRUCTION_TYPE::ADD},
    {"sll", alu_instruction_t::ALU_INSTRUCTION_TYPE::SLL},
    {"slt", alu_instruction_t::ALU_INSTRUCTION_TYPE::SLT},
    {"sltu", alu_instruction_t::ALU_INSTRUCTION_TYPE::SLTU},
    {"xor", alu_instruction_t::ALU_INSTRUCTION_TYPE::XOR},
    {"srl", alu_instruction_t::ALU_INSTRUCTION_TYPE::SRL},
    {"sra", alu_instruction_t::ALU_INSTRUCTION_TYPE::SRA},
    {"or",  alu_instruction_t::ALU_INSTRUCTION_TYPE::OR},
    {"and",alu_instruction_t::ALU_INSTRUCTION_TYPE::AND}
};
const std::unordered_map<std::string,branch_instruction_t::BRANCH_INSTRUCTION_TYPE>   
lookup_t::_branch_instructions = {
    {"beq",branch_instruction_t::BRANCH_INSTRUCTION_TYPE::BEQ},
    {"bne",branch_instruction_t::BRANCH_INSTRUCTION_TYPE::BNE},
    {"blt",branch_instruction_t::BRANCH_INSTRUCTION_TYPE::BLT},
    {"bge",branch_instruction_t::BRANCH_INSTRUCTION_TYPE::BGE},
    {"bltu",branch_instruction_t::BRANCH_INSTRUCTION_TYPE::BLTU},
    {"bgeu",branch_instruction_t::BRANCH_INSTRUCTION_TYPE::BGEU}
};
const std::unordered_map<std::string,jump_instruction_t::JUMP_INSTRUCTION_TYPE>       
lookup_t::_jump_instructions = {
    {"jalr",jump_instruction_t::JUMP_INSTRUCTION_TYPE::JALR},
    {"jal",jump_instruction_t::JUMP_INSTRUCTION_TYPE::JAL}
};
reg_id_t lookup_t::reg_id(const std::string& reg_string_id) {
	if (_registers.find(reg_string_id) == _registers.end())
		return 255;
	return _registers.at(reg_string_id);
}

load_instruction_t::LOAD_INSTRUCTION_TYPE 
lookup_t::load_type(const std::string& str) {
	if (_load_instructions.find(str) == _load_instructions.end())
		return load_instruction_t::LOAD_INSTRUCTION_TYPE::UNKNOWN;
	return _load_instructions.at(str);
}

store_instruction_t::STORE_INSTRUCTION_TYPE 
lookup_t::store_type(const std::string& str) {
	if (_store_instructions.find(str) == _store_instructions.end())
		return store_instruction_t::STORE_INSTRUCTION_TYPE::UNKNOWN;
	return _store_instructions.at(str);
}

alu_instruction_t::ALU_INSTRUCTION_TYPE 
lookup_t::alui_type(const std::string& str) {
	if (_alui_instructions.find(str) == _alui_instructions.end())
		return alu_instruction_t::ALU_INSTRUCTION_TYPE::UNKNOWN;
	return _alui_instructions.at(str);
}

alu_instruction_t::ALU_INSTRUCTION_TYPE 
lookup_t::alur_type(const std::string& str) {
	if (_alur_instructions.find(str) == _alur_instructions.end())
		return alu_instruction_t::ALU_INSTRUCTION_TYPE::UNKNOWN;
	return _alur_instructions.at(str);
}

branch_instruction_t::BRANCH_INSTRUCTION_TYPE 
lookup_t::branch_type(const std::string& str) {
	if (_branch_instructions.find(str) == _branch_instructions.end())
		return branch_instruction_t::BRANCH_INSTRUCTION_TYPE::UNKNOWN;
	return _branch_instructions.at(str);
}

jump_instruction_t::JUMP_INSTRUCTION_TYPE 
lookup_t::jump_type(const std::string& str) {
	if (_jump_instructions.find(str) == _jump_instructions.end())
		return jump_instruction_t::JUMP_INSTRUCTION_TYPE::UNKNOWN;
	return _jump_instructions.at(str);
}

bool lookup_t::is_imm(const std::string& s) {
    if (s.empty()) return false;
    // Hexadecimal: 0x or 0X prefix
    if (s.size() > 2 && s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) {
        for (size_t i = 2; i < s.size(); ++i) {
            char c = s[i];
            if (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))) {
                return false;
            }
        }
        return true;
    }
    // Decimal: optional leading '-' and digits
    size_t i = 0;
    if (s[0] == '-') i = 1;
    if (i == s.size()) return false;
    for (; i < s.size(); ++i) {
        if (!std::isdigit(static_cast<unsigned char>(s[i]))) {
            return false;
        }
    }
    return true;
}