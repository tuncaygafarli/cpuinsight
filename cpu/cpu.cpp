#include "cpu.h"
#include "instruction.h"
#include <iostream>
#include <iomanip>

void CPU::d_cache_commit(memory_addr_t mem_addr, data_t data) {
	if (_d_cache.find(mem_addr) == _d_cache.end()) {
		_cycles += CACHE_MISS_PENALTY;
		_d_cache.emplace(mem_addr, data);
	}
	else
		_d_cache[mem_addr] = data;
}

void CPU::reg_file_commit(const reg_id_t& reg_id, data_t data) {
	if (reg_id >= 32) {
		std::cout << "Tried to write to a non-existing register";
		return;
	}
	if (reg_id == 0)
		return;
	if (_reg_file.find(reg_id) == _reg_file.end())
		_reg_file.emplace(reg_id, data);
	else
		_reg_file[reg_id] = data;
}

void CPU::jump_to_label(label_id_t label_) {
	size_t program_size = _program.size();
	for (size_t i = 0; i < program_size; ++i) {
		if (_program[i]->is_label_instruction() && _program[i]->target_label() == label_) {
			_pc = static_cast<memory_addr_t>(i);
			return;
		}
	}
}

void CPU::update_pc(memory_addr_t next_pc_val) {
	_pc = next_pc_val;
}

void CPU::load_program(cpu_program_t&& program_) {
	_program = std::move(program_);
}

void CPU::log(std::ostream& os)  {
	const size_t COLS = 4;

	const int VAL_WIDTH = 12;

	os << "--------------------------------------------------------------\n";
	os << "CPU Register File (Signed Values) \n";
	os << "--------------------------------------------------------------\n";

	for (size_t reg_id = 0; reg_id < 32; reg_id++) {
		os << std::left << "x" << std::setw(2) << reg_id << ": ";

		os << std::right << std::setw(VAL_WIDTH)
			<< _reg_file[static_cast<reg_id_t>(reg_id)]._signed;

		// Check if we need to start a new line
		if ((reg_id + 1) % COLS == 0) {
			os << "\n";
		}
		else {
			os << "  |  ";
		}
	}

	if (32 % COLS != 0) {
		os << "\n";
	}

	os << "--------------------------------------------------------------\n";
	os << "Total Branch Executions : " << _total_branches << "\n";
	os << "Correct Predictions : " << _correct_predictions << "\n";
	os << "Total Mispredictions : " << _total_branches - _correct_predictions << "\n";
	os << "Prediction Accuracy : " << static_cast<float>(_correct_predictions) / static_cast<float>(_total_branches) * 100.0f << "%\n";
	os << "Total cycles : " << _cycles << "\n";
	os << "--------------------------------------------------------------\n";
}

void CPU::incr_correct_predictions() {
	_correct_predictions++;
}

void CPU::incr_total_branches() {
	_total_branches++;
}

void CPU::penalty() {
	_cycles += BRANCH_MISPREDICTION_PENALTY;
}
const memory_addr_t& CPU::get_pc() const {
	return _pc;
}

const uint64_t& CPU::get_cycles() const {
	return _cycles;
}

const uint64_t& CPU::get_total_branches() const {
	return _total_branches;
}

const uint64_t& CPU::get_correct_predictions() const {
	return _correct_predictions;
}

const cpu_dcache_t& CPU::get_dcache() const {
	return _d_cache;
}

const data_t& CPU::d_cache_read(memory_addr_t addr) {
	if (_d_cache.find(addr) == _d_cache.end()) {
		_cycles += CACHE_MISS_PENALTY;
		return _reg_file[0];
	}
	return _d_cache[addr];
}

const data_t& CPU::reg_file_read(const reg_id_t& reg_id)  {
	if (_reg_file.find(reg_id) == _reg_file.end() || reg_id == 0)
		return _reg_file[0];
	return _reg_file[reg_id];
}

CPU::CPU(CPU::PREDICTOR_TYPE type) {
	switch (type) {
	case(CPU::PREDICTOR_TYPE::SIMPLE):
		_branch_predictor = std::make_unique<simple_predictor_t>();
		break;
	case(CPU::PREDICTOR_TYPE::GAg):
		_branch_predictor = std::make_unique<GAg_predictor_t>();
		break;
	case(CPU::PREDICTOR_TYPE::PAg):
		_branch_predictor = std::make_unique<PAg_predictor_t>();
		break;
	case(CPU::PREDICTOR_TYPE::GSHARE):
		_branch_predictor = std::make_unique<gshare_predictor_t>();
		break;
		// shouldnt happen
	default:
		break;
	}
	for (reg_id_t i = 0; i < 32; i++) {
		_reg_file[i]._unsigned = 0ull;
	}
	_program = cpu_program_t();
	_d_cache = cpu_dcache_t();
}

void CPU::execute() {
	if (_pc >= _program.size() && !_halt) {
		_halt = true;
		return;
	}
	if (_program[_pc]->is_label_instruction()) {
		_pc++;
	}
	auto old_pc = _pc;
	_program[_pc]->execute(*this);
	if (old_pc == _pc)
		_pc++;
	_cycles++;
}

void CPU::update_bht(branch_instruction_id_t branch_label, bool branch_direction) {
	_branch_predictor->update(branch_label, branch_direction);
}

bool CPU::predict_branch(branch_instruction_id_t branch_label) const {
	return _branch_predictor->predict(branch_label);
}

void CPU::reset() {
	_pc = 0;
	_d_cache.clear();
	_reg_file.clear();
	_total_branches = 0;
	_cycles = 0;
	_correct_predictions = 0;
	_halt = false;
}
bool CPU::endofprogram() const {
	return _halt;
}

const cpu_reg_file_t& CPU::get_reg_file() const {
	return _reg_file;
}
