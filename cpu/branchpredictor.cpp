#include "branchpredictor.h"

void simple_predictor_t::update(branch_instruction_id_t branch_id, bool branch_taken) {
	branch_history_t branch_history = _bht[branch_id];
	if (branch_taken) { 
		// use of 2 bits 
		if (branch_history >= 0 && branch_history < 3)
			_bht[branch_id] = branch_history++;
	}
	else {
		if (branch_history >= 1)
			_bht[branch_id] = branch_history--;
	}
}

bool simple_predictor_t::predict([[maybe_unused]] branch_instruction_id_t branch_id) {
	if (_bht.find(branch_id) == _bht.end()) {
		_bht.emplace(branch_id,0);
		return false;
	}
	branch_history_t branch_history = _bht[branch_id];
	// use saturating counter
	if (branch_history >= 2)
		return true;
	return false;
}

void GAg_predictor_t::update([[maybe_unused]] branch_instruction_id_t branch_id, bool branch_taken) {
	branch_history_t branch_history = _pattern_history_table[_branch_shift_register];
    
	if (branch_taken) {
		if (branch_history >= 0 && branch_history < 3)
			branch_history++;
	}
	else {
		if (branch_history > 0) 
			branch_history--;
	}
    
	_pattern_history_table[_branch_shift_register] = branch_history;
    
 	_branch_shift_register <<= 1;
    if (branch_taken) {
		_branch_shift_register |= 1;
	}
	_branch_shift_register &= (1 << SHIFT_REGISTER_SIZE) - 1;
}

bool GAg_predictor_t::predict([[maybe_unused]] branch_instruction_id_t branch_id) {

	if(_pattern_history_table.find(_branch_shift_register) == _pattern_history_table.end()) {
		_pattern_history_table.emplace(_branch_shift_register, 2); // start at weakly taken
		return true;
	}
	branch_history_t branch_history = _pattern_history_table[_branch_shift_register];
	if(branch_history >= 2)
		return true;
	return false;
}

void PAg_predictor_t::update(branch_instruction_id_t branch_id, bool branch_taken) {
	uint64_t branch_shift_register = _branch_history_shift_register_table[branch_id];
	branch_history_t branch_history = _pattern_history_table[branch_shift_register];
    
	if (branch_taken) {
		if (branch_history >= 0 && branch_history < 3)
			branch_history++;
	}
	else {
		if (branch_history > 0) 
			branch_history--;
	}
    
	_pattern_history_table[branch_shift_register] = branch_history;
    
 	branch_shift_register <<= 1;
    if (branch_taken) {
		branch_shift_register |= 1;
	}
	branch_shift_register &= (1 << SHIFT_REGISTER_SIZE) - 1;
	_branch_history_shift_register_table[branch_id] = branch_shift_register;
}

bool PAg_predictor_t::predict(branch_instruction_id_t branch_id) {

	uint64_t branch_shift_register = 0;
	if(_branch_history_shift_register_table.find(branch_id) != _branch_history_shift_register_table.end()) {
		branch_shift_register  = _branch_history_shift_register_table[branch_id];
	}
	if(_pattern_history_table.find(branch_shift_register) == _pattern_history_table.end()) {
		_pattern_history_table.emplace(branch_shift_register, 2); // start at weakly taken
		return true;
	}
	branch_history_t branch_history = _pattern_history_table[branch_shift_register];
	if(branch_history >= 2)
		return true;
	return false;
}

void gshare_predictor_t::update(branch_instruction_id_t branch_id, bool branch_taken) {

	uint64_t pht_index = _branch_shift_register ^ branch_id;
	branch_history_t branch_history = _pattern_history_table[pht_index];
    
	if (branch_taken) {
		if (branch_history >= 0 && branch_history < 3)
			branch_history++;
	}
	else {
		if (branch_history > 0) 
			branch_history--;
	}
    
	_pattern_history_table[pht_index] = branch_history;
    
 	_branch_shift_register <<= 1;
    if (branch_taken) {
		_branch_shift_register |= 1;
	}
	_branch_shift_register &= (1 << SHIFT_REGISTER_SIZE) - 1;
}

bool gshare_predictor_t::predict(branch_instruction_id_t branch_id) {

	uint64_t pht_index = _branch_shift_register ^ branch_id;
	if(_pattern_history_table.find(pht_index) == _pattern_history_table.end()) {
		_pattern_history_table.emplace(pht_index, 2); // start at weakly taken
		return true;
	}
	branch_history_t branch_history = _pattern_history_table[pht_index];
	if(branch_history >= 2)
		return true;
	return false;
}
