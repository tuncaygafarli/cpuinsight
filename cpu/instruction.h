#pragma once
#include "../aliases.h"
#include <string>
class CPU;
struct instruction_t {
    virtual void execute(CPU &cpu) = 0;
    virtual label_id_t target_label() { return no_label; }
    virtual bool is_label_instruction() { return false; }
    virtual ~instruction_t() = default;
};
struct memory_instruction_t : instruction_t {
    void execute(CPU &cpu) override = 0;
    memory_instruction_t(
        const reg_id_t  dest_reg_, 
        const reg_id_t  base_reg_, 
        offset_t offset_
    ) : 
        _dest_src_reg(dest_reg_),
        _base_reg(base_reg_),
        _offset(offset_) {}
protected:
    reg_id_t _dest_src_reg;
    reg_id_t _base_reg;
    offset_t _offset;
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
    };
    void execute(CPU &cpu) override;
    load_instruction_t(
        LOAD_INSTRUCTION_TYPE type_,
        const reg_id_t dest_reg_, 
        offset_t offset_,
        const reg_id_t base_reg_
    ) : 
        memory_instruction_t(dest_reg_,base_reg_,offset_), _type(type_) {}
private:
    LOAD_INSTRUCTION_TYPE _type;
};
struct store_instruction_t : memory_instruction_t {
    enum class STORE_INSTRUCTION_TYPE
    {
        SB,
        SH,
        SW,
        UNKNOWN
    };
    void execute(CPU &cpu) override;
    store_instruction_t(
        STORE_INSTRUCTION_TYPE type_,
        const reg_id_t dest_reg_, 
        offset_t offset_,
        const reg_id_t base_reg_
    ) : 
        memory_instruction_t(dest_reg_,base_reg_,offset_), _type(type_) {}
private:
    STORE_INSTRUCTION_TYPE _type;
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
    };
    alu_instruction_t( 
        ALU_INSTRUCTION_TYPE type_,
        const reg_id_t dest_reg_, 
        const reg_id_t src1_reg_, 
        int64_t src2_val,
        bool is_imm_
    ) : 
        _dest_reg(dest_reg_),
        _src1_reg(src1_reg_),
        _type(type_),
        _is_imm(is_imm_) {

        if (_is_imm) 
            _src2.imm_val = src2_val;
         else 
            _src2.src2_reg = (reg_id_t)src2_val; 
        
    }
    void execute(CPU &cpu) override;
private:
    _src2_ _src2;
    reg_id_t _dest_reg;
    reg_id_t _src1_reg;
    ALU_INSTRUCTION_TYPE _type;
    bool _is_imm = false;
};
struct load_upper_imm_instruction_t : instruction_t {
    load_upper_imm_instruction_t(reg_id_t dest_reg_,int64_t upimm_) : _dest_reg(dest_reg_),_upimm(upimm_) {}
    void execute(CPU &cpu) override;
private:
    reg_id_t _dest_reg;
    int64_t _upimm;
}; 
struct auipc_instruction_t : instruction_t {
    auipc_instruction_t(reg_id_t dest_reg_,int64_t upimm_) : _dest_reg(dest_reg_),_upimm(upimm_) {}
    void execute(CPU &cpu) override;
private:
    reg_id_t _dest_reg;
    int64_t _upimm;
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
    } _type;
    branch_instruction_t(
        BRANCH_INSTRUCTION_TYPE type_,
        reg_id_t src1_,
        reg_id_t src2_,
        label_id_t label_,
        const std::string& instr_str,
        branch_instruction_id_t branch_id
    ) :
        _src1_reg(src1_),
        _src2_reg(src2_),
        _label_id(label_),
        _type(type_),
        _instruction_str(instr_str),
        _id(branch_id)
        {}
    void execute(CPU &cpu) override;
    label_id_t target_label() override { return _label_id; }
    void set_target_label(label_id_t label_id) { _label_id = label_id; };

private:
    branch_instruction_id_t _id;
    std::string _instruction_str;
    reg_id_t _src1_reg; 
    reg_id_t _src2_reg;
    label_id_t _label_id;
};
struct jump_instruction_t : instruction_t {
    enum class JUMP_INSTRUCTION_TYPE
    {
        JALR,
        JAL,
        UNKNOWN
    } _type;
    void execute(CPU &cpu) override;
    label_id_t target_label() override { return _label_id; }
    void set_target_label(label_id_t label_id) { _label_id = label_id; };
    jump_instruction_t (
        JUMP_INSTRUCTION_TYPE type_,
        reg_id_t dest_reg_,
        reg_id_t src1_,
        label_id_t label_id_,
        int64_t imm_
    ) : _type(type_),
        _dest_reg(dest_reg_),
        _src1(src1_),
        _label_id(label_id_),
        _imm(imm_) {}
private:
    reg_id_t _dest_reg;
    label_id_t _label_id; // jal uses this
    reg_id_t _src1;// jalr uses this
    int64_t _imm;  // jalr uses this
};
struct label_instruction_t :  instruction_t {
    label_instruction_t(label_id_t label_) : _label_id(label_) {}
    label_id_t target_label() override { return _label_id; }
    void execute(CPU &cpu) override {} 
    bool is_label_instruction() override { return true; }
private:
    label_id_t _label_id;
};