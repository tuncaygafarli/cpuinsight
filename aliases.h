#pragma once

#include <cstdint>
#include <unordered_map>
#include <vector>
#include <memory>
#include <string>

auto constexpr NO_LABEL = 0;
auto constexpr FORWARD_LABEL = 1;
auto constexpr INVALID_REG_ID = 255;
auto constexpr SHIFT_REGISTER_SIZE = 8;
auto constexpr FSM_STATE_BITS = 2;
auto constexpr BRANCH_MISPREDICTION_PENALTY = 3;
auto constexpr CACHE_MISS_PENALTY = 100;
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
using cpu_program_t = std::vector<std::unique_ptr<instruction_t>>;
using cpu_dcache_t = std::unordered_map<memory_addr_t, data_t>;
using cpu_reg_file_t = std::unordered_map<reg_id_t, data_t>;
using branch_history_t = uint8_t;
using branch_history_table_t = std::unordered_map<branch_instruction_id_t, branch_history_t>;