#include "instruction.h"
#include "cpu.h"
void load_instruction_t::execute(CPU& cpu) {
    // get the base register value
    data_t base_reg_val = cpu.reg_file_read(base_reg);
    // calculate the addr
    memory_addr_t addr = base_reg_val._unsigned + offset;
    // read from addr
    data_t read_val = cpu.d_cache_read(addr); // this is the val that is going to be stored at destination register

    switch(type) {
        case LOAD_INSTRUCTION_TYPE::LB: 
            read_val._signed &= (0x000000000000000F);
            break;
        case LOAD_INSTRUCTION_TYPE::LH:
            read_val._signed &= (0x00000000FFFFFFFF);
            break;
        case LOAD_INSTRUCTION_TYPE::LBU:
            read_val._unsigned &= (0x000000000000000F);
            break;
        case LOAD_INSTRUCTION_TYPE::LHU:
            read_val._unsigned &= (0x00000000FFFFFFFF);
            break;
        case LOAD_INSTRUCTION_TYPE::LW:
            break;
    }
    cpu.reg_file_commit(dest_src_reg, read_val);
}

void store_instruction_t::execute(CPU& cpu) {
    // get the source val
    data_t source_reg_val = cpu.reg_file_read(dest_src_reg);
    // calculate the addr
    data_t base_reg_val = cpu.reg_file_read(base_reg);
    memory_addr_t addr = base_reg_val._unsigned + offset;

    data_t commit_val = source_reg_val;
    switch(type) {
        case STORE_INSTRUCTION_TYPE::SB:
            commit_val._unsigned &= (0x000000000000000F);
            break;
        case STORE_INSTRUCTION_TYPE::SH:
            commit_val._unsigned &= (0x00000000FFFFFFFF);
            break;
        case STORE_INSTRUCTION_TYPE::SW:
            break;
        default:
            break;
    }
    cpu.d_cache_commit(addr, commit_val);
}

void alu_instruction_t::execute(CPU& cpu) {
    data_t src1_val = cpu.reg_file_read(src1_reg);
    data_t src2_val = data_t();
    if(is_imm)
        src2_val._signed = src2.imm_val;
    else
        src2_val = cpu.reg_file_read(src2.src2_reg);
    data_t result = data_t();
    
    uint64_t shift_amount = src2_val._unsigned;

    switch (type) {
    case ALU_INSTRUCTION_TYPE::ADD:
        result._signed = src1_val._signed + src2_val._signed;
        break;
    case ALU_INSTRUCTION_TYPE::SUB:
        result._signed = src1_val._signed - src2_val._signed;
        break;
    case ALU_INSTRUCTION_TYPE::SLL:
        result._unsigned = src1_val._unsigned << shift_amount;
        break;
    case ALU_INSTRUCTION_TYPE::SLT:
        result._signed = static_cast<int64_t>(src1_val._signed < src2_val._signed);
        break;
    case ALU_INSTRUCTION_TYPE::SLTU:
        result._signed = static_cast<int64_t>(src1_val._unsigned < src2_val._unsigned);
        break;
    case ALU_INSTRUCTION_TYPE::XOR:
        result._unsigned = (src1_val._unsigned ^ src2_val._unsigned);
        break;
    case ALU_INSTRUCTION_TYPE::SRL:
        result._unsigned = src1_val._unsigned >> shift_amount;
        break;
     case ALU_INSTRUCTION_TYPE::SRA:
        result._signed = src1_val._signed >> shift_amount; // this uses the sign-extending behavior
        break;
    case ALU_INSTRUCTION_TYPE::OR:
        result._unsigned = src1_val._unsigned | src2_val._unsigned;
        break;
    case ALU_INSTRUCTION_TYPE::AND:
        result._unsigned = src1_val._unsigned & src2_val._unsigned;
        break;
    default:
        break;
    }
    cpu.reg_file_commit(dest_reg, result);
}

void load_upper_imm_instruction_t::execute(CPU& cpu) {
    data_t commit_data = {upimm & 0xFFFFFFFFFF000000};
    cpu.reg_file_commit(dest_reg,commit_data);
}

void branch_instruction_t::execute(CPU& cpu) {
    data_t src1_val = cpu.reg_file_read(src1_reg);
    data_t src2_val = cpu.reg_file_read(src2_reg);
    bool should_branch = false;
    switch (type) {
    case BRANCH_INSTRUCTION_TYPE::BEQ:
        should_branch = src1_val._signed == src2_val._signed;
        break;
    case BRANCH_INSTRUCTION_TYPE::BNE:
        should_branch = src1_val._signed != src2_val._signed;
        break;
    case BRANCH_INSTRUCTION_TYPE::BLT:
        should_branch = src1_val._signed < src2_val._signed;
        break;
    case BRANCH_INSTRUCTION_TYPE::BGE:
        should_branch = src1_val._signed >= src2_val._signed;
        break;
    case BRANCH_INSTRUCTION_TYPE::BLTU:
        should_branch = src1_val._unsigned < src2_val._unsigned;
        break;
    case BRANCH_INSTRUCTION_TYPE::BGEU:
        should_branch = src1_val._unsigned >= src2_val._unsigned;
        break;
    default:
        break;
    }
    if (should_branch) cpu.jump_to_label(label_id);
}

void jump_instruction_t::execute(CPU& cpu) {
    memory_addr_t pc_next = cpu.get_pc() + 1;
    switch (type) {
    case JUMP_INSTRUCTION_TYPE::JAL:
        cpu.jump_to_label(label_id);
        break;
    case JUMP_INSTRUCTION_TYPE::JALR:
        data_t rs1_val = cpu.reg_file_read(src1);
        cpu.update_pc(rs1_val._unsigned + imm);
        break;

    }
    cpu.reg_file_commit(dest_reg, { pc_next });
}

void auipc_instruction_t::execute(CPU& cpu) {
    int64_t pc_val = cpu.get_pc();
    int64_t val = upimm & (0xFFFFFFFFFF000000);
    data_t commit_data = data_t();
    commit_data._signed = pc_val + val;
    cpu.reg_file_commit(dest_reg, commit_data);
}
