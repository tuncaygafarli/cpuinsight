#include "../aliases.h"
#include "branchpredictor.h"
#include <ostream>
#pragma  once
class CPU {
public:
	enum class PREDICTOR_TYPE {
		SIMPLE,
		GAg,
		PAg,
		GSHARE
	};
	CPU(PREDICTOR_TYPE type);
	void 			  		execute();
	void 			  		reset();
	bool 			  		endofprogram() const;
	const cpu_reg_file_t& 	get_reg_file() const ;
	void			  		load_program(cpu_program_t&& program_);
	void			  		update_bht(branch_instruction_id_t branch_label, bool branch_direction);
    bool			  		predict_branch(branch_instruction_id_t branch_id) const;
	void			  		d_cache_commit(memory_addr_t mem_addr, data_t data);
	void			  		reg_file_commit(const reg_id_t& reg_id, data_t data);
	const data_t&	  		d_cache_read(memory_addr_t addr);
	const data_t&	  		reg_file_read(const reg_id_t& reg_id);
	void			  		jump_to_label(label_id_t label_);
	void			  		update_pc(memory_addr_t next_pc_val);
	void			  		log(std::ostream& os);
	void			  		incr_correct_predictions();
	void			  		incr_total_branches();
	void			  		penalty();
	const memory_addr_t&	get_pc() const;
	const uint64_t& 		get_cycles() const;
	const uint64_t&  		get_total_branches() const;
	const uint64_t&   		get_correct_predictions() const;
	const cpu_dcache_t& 	get_dcache() const;

private:
	bool _halt = false;
	memory_addr_t _pc = 0; // program counter
	cpu_program_t _program;
	cpu_dcache_t _d_cache;
	cpu_reg_file_t _reg_file;
	std::unique_ptr<branch_predictor_t> _branch_predictor;
	uint64_t _correct_predictions = 0;
	uint64_t _total_branches = 0;
	uint64_t _cycles = 0;
};