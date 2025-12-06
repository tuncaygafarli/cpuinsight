#include "../common/aliases.h"
#pragma  once
class CPU {

public:
	void			d_cache_commit(memory_addr_t mem_addr, data_t data);
	void			reg_file_commit(const reg_id_t& reg_id, data_t data);
	data_t			d_cache_read(memory_addr_t addr);
	data_t			reg_file_read(const reg_id_t& reg_id);
	void			jump_to_label(label_id_t label_);
	void			update_pc(memory_addr_t next_pc_val);
	memory_addr_t	get_pc();
private:
	memory_addr_t _pc; // program counter
	program_t _program;
	d_cache_t _d_cache;
	reg_file_t _reg_file;
};