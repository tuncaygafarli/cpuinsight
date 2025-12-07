#pragma once
#include <string>
enum class TOKEN_TYPE {
	LOAD_OPERATION,
	STORE_OPERATION,
	ALU_OPERATION_R,
	ALU_OPERATION_I,
	BRANCH_OPERATION,
	JUMP_OPERATION,
	LOAD_UPPER,
	AUIPC,
	REGISTER,
	IMMEDIATE,
	LABEL,
	COMMA,
	LPAREN,
	RPAREN,
	IDENTIFIER,
	NEW_LINE
};
struct token_t {
	TOKEN_TYPE type;
	std::string word;
	token_t(const std::string& _word, const TOKEN_TYPE _type) : word(_word), type(_type) {}
};
