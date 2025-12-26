#pragma once
#include <string>

#include "../cpu/cpu.h" 

namespace Helpers {
	std::string trim_instruction(const std::string& str);
	std::string id_t_to_string(uint8_t reg_id);
	std::string data_t_to_string(const data_t& data);
};