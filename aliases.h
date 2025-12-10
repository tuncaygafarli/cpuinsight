#pragma once

#include <cstdint>
#include <unordered_map>
#include <vector>
#include <memory>
#include <string>

auto constexpr no_label = 0;
auto constexpr forward_label = 1;
auto constexpr invalid_reg_id = 255;
union data_t {
	uint64_t _unsigned;
	int64_t  _signed;
};
struct instruction_t;
using memory_addr_t = uint64_t;
using offset_t = int64_t;
using reg_id_t = uint8_t;
using label_id_t = uint64_t;
using branch_instruction_id_t = uint64_t;
using program_t = std::vector<std::unique_ptr<instruction_t>>;
using d_cache_t = std::unordered_map<memory_addr_t, data_t>;
using reg_file_t = std::unordered_map<reg_id_t, data_t>;
using branch_history_t = uint8_t;
using branch_lookup_t = std::unordered_map<branch_instruction_id_t, std::pair<const std::string*,branch_history_t>>;