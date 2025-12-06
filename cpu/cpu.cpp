#include "cpu.h"
#include "instruction.h"

void CPU::d_cache_commit(memory_addr_t mem_addr, data_t data) {
	if (_d_cache.find(mem_addr) == _d_cache.end()) 
		_d_cache.emplace(mem_addr, data);
	else 
		_d_cache[mem_addr] = data;
}

void CPU::reg_file_commit(const reg_id_t& reg_id, data_t data) {
	if (_reg_file.find(reg_id) == _reg_file.end())
		_reg_file.emplace(reg_id, data);
	else
		_reg_file[reg_id] = data;
}

void CPU::jump_to_label(label_id_t label_) {
	size_t program_size = _program.size();
	for (size_t i = 0; i < program_size; ++i) {
		if (_program[i]->label() == label_) {
			_pc = static_cast<memory_addr_t>(i + 1);
			return;
		}
	}
}

void CPU::update_pc(memory_addr_t next_pc_val) {
	_pc = next_pc_val;
}

memory_addr_t CPU::get_pc() {
	return _pc;
}

data_t CPU::d_cache_read(memory_addr_t addr) {
	if (_d_cache.find(addr) == _d_cache.end())
		return { 0l };
	return _d_cache[addr];
}

data_t CPU::reg_file_read(const reg_id_t& reg_id) {
	if (_reg_file.find(reg_id) == _reg_file.end())
		return { 0l };
	return _reg_file[reg_id];
}

