#pragma once
#include "../aliases.h"
class CPU;
struct instruction_t {
    virtual void execute(CPU &cpu) = 0;
    virtual label_id_t label() { return no_label; }
    virtual bool is_label_instruction() { return false; }
    virtual ~instruction_t() = default;
};
struct memory_instruction_t : instruction_t {
    reg_id_t dest_src_reg;
    reg_id_t base_reg;
    offset_t offset;
    void execute(CPU &cpu) override = 0;
    memory_instruction_t(
        const reg_id_t  dest_reg_, 
        const reg_id_t  base_reg_, 
        offset_t offset_
    ) : 
        dest_src_reg(dest_reg_),
        base_reg(base_reg_),
        offset(offset_) {}
};
struct load_instruction_t : memory_instruction_t {
    enum class LOAD_INSTRUCTION_TYPE
    {
        LB,
        LH,
        LW,
        LBU,
        LHU,
        UNKNOWN
    } type;
    void execute(CPU &cpu) override;
    load_instruction_t(
        LOAD_INSTRUCTION_TYPE type_,
        const reg_id_t dest_reg_, 
        offset_t offset_,
        const reg_id_t base_reg_
    ) : 
        memory_instruction_t(dest_reg_,base_reg_,offset_), type(type_) {}
};
struct store_instruction_t : memory_instruction_t {
    enum class STORE_INSTRUCTION_TYPE
    {
        SB,
        SH,
        SW,
        UNKNOWN
    } type;
    void execute(CPU &cpu) override;
    store_instruction_t(
        STORE_INSTRUCTION_TYPE type_,
        const reg_id_t dest_reg_, 
        offset_t offset_,
        const reg_id_t base_reg_
    ) : 
        memory_instruction_t(dest_reg_,base_reg_,offset_), type(type_) {}
};
struct alu_instruction_t : instruction_t {
    enum class ALU_INSTRUCTION_TYPE
    {
        ADD,
        SUB,
        SLL,
        SLT,
        SLTU,
        XOR,
        SRL,
        SRA,
        OR,
        AND,
        MUL,
        MULH,
        MULHSU,
        MULHU,
        DIV,
        DIVU,
        REM,
        REMU,
        UNKNOWN
    };
    union _src2_ {
        reg_id_t src2_reg;
        int64_t imm_val;
    } src2;
    reg_id_t dest_reg;
    reg_id_t src1_reg;
    ALU_INSTRUCTION_TYPE type;
    bool is_imm = false;
    alu_instruction_t( 
        ALU_INSTRUCTION_TYPE type_,
        const reg_id_t dest_reg_, 
        const reg_id_t src1_reg_, 
        int64_t src2_val,
        bool is_imm_
    ) : 
        dest_reg(dest_reg_),
        src1_reg(src1_reg_),
        type(type_),
        is_imm(is_imm_) {

        if (is_imm) 
            src2.imm_val = src2_val;
         else 
            src2.src2_reg = (reg_id_t)src2_val; 
        
    }
    void execute(CPU &cpu) override;
};
struct load_upper_imm_instruction_t : instruction_t {
    reg_id_t dest_reg;
    int64_t upimm;
    load_upper_imm_instruction_t(reg_id_t dest_reg_,int64_t upimm_) : dest_reg(dest_reg_),upimm(upimm_) {}
    void execute(CPU &cpu) override;
}; 
struct auipc_instruction_t : instruction_t {
    reg_id_t dest_reg;
    int64_t upimm;
    auipc_instruction_t(reg_id_t dest_reg_,int64_t upimm_) : dest_reg(dest_reg_),upimm(upimm_) {}
    void execute(CPU &cpu) override;
};
struct branch_instruction_t : instruction_t {
    enum class BRANCH_INSTRUCTION_TYPE
    {
        BEQ,
        BNE,
        BLT,
        BGE,
        BLTU,
        BGEU,
        UNKNOWN
    } type;
    reg_id_t src1_reg;
    reg_id_t src2_reg;
    label_id_t label_id;
    branch_instruction_t(
        BRANCH_INSTRUCTION_TYPE type_,
        reg_id_t src1_,
        reg_id_t src2_,
        label_id_t label_
    ) :
        src1_reg(src1_),
        src2_reg(src2_),
        label_id(label_),
        type(type_)
        {}
    void execute(CPU &cpu) override;
    label_id_t label() override { return label_id; }
};
struct jump_instruction_t : instruction_t {
    enum class JUMP_INSTRUCTION_TYPE
    {
        JALR,
        JAL,
        UNKNOWN
    } type;
    reg_id_t dest_reg;
    label_id_t label_id; // jal uses this
    reg_id_t src1;// jalr uses this
    int64_t imm;  // jalr uses this
    void execute(CPU &cpu) override;
    label_id_t label() override { return label_id; }
    jump_instruction_t (
        JUMP_INSTRUCTION_TYPE type_,
        reg_id_t dest_reg_,
        reg_id_t src1_,
        label_id_t label_id_,
        int64_t imm_
    ) : type(type_),
        dest_reg(dest_reg_),
        src1(src1_),
        label_id(label_id_),
        imm(imm_) {}
};
struct label_instruction_t :  instruction_t {
    label_id_t label_id;
    label_instruction_t(label_id_t label_) : label_id(label_) {}
    label_id_t label() override { return label_id; }
    void execute(CPU &cpu) override {} 
    bool is_label_instruction() override { return true; }
};