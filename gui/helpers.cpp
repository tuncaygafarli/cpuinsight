#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <sstream>
#include <iomanip>
#include <string>

#include "helpers.h"
#include "../cpu/cpu.h" 

std::string Helpers::trim_instruction(const std::string& str) {
	auto start = str.find_first_not_of(" \t\n\r");
	if (start == std::string::npos) return "";

	auto end = str.find_last_not_of(" \t\n\r");
	return str.substr(start, end - start + 1);
}

std::string Helpers::id_t_to_string(uint8_t reg_id) {
	std::stringstream ss;
	ss << "0x" << std::hex << std::uppercase
		<< std::setw(2) << std::setfill('0')
		<< static_cast<int>(reg_id);
	return ss.str();
}

std::string Helpers::data_t_to_string(const data_t& data) {
	std::stringstream ss;

	ss << "0x" << std::hex << std::uppercase
		<< std::setw(16) << std::setfill('0') << data._unsigned;

	return ss.str();
}