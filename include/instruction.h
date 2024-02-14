#ifndef _INSTRUCTION_H_
#define _INSTRUCTION_H_

#include "common.h"

#define NO_FLAGS "----"

typedef enum  {
    am_imp,
    am_r_d16,
    am_r_r, 
    am_mr_r,
    am_r, 
    am_r_d8,
    am_r_mr,
    am_r_hli,
    am_r_hld,
    am_hli_r,
    am_hld_r,
    am_r_a8,
    am_a8_r,
    am_hl_spr,
    am_d16,
    am_d8,
    am_d16_r,
    am_mr_d8,
    am_mr, 
    am_a16_r, 
    am_r_a16,
    am_none
} addr_mode;

typedef enum  {
    in_ld,
    in_ldh, 
    in_push,
    in_pop,
    in_add, 
    in_adc,
    in_sub,
    in_sbc,
    in_cp,
    in_inc,
    in_dec, 
    in_and,
    in_or,
    in_xor,
    in_ccf,
    in_scf,
    in_daa,
    in_cpl,
    in_jp,
    in_jr,
    in_call,
    in_ret,
    in_reti,
    in_rst,
    in_halt, 
    in_stop,
    in_di,
    in_ei,
    in_nop,

    // todo
    in_rlca,
    in_rrca, 
    in_rla, 
    in_rra,

    // prefix cb instruction
    in_cb,
} mnemonic;

typedef enum  {
    reg_a,
    reg_f,
    reg_b, 
    reg_c,
    reg_d,
    reg_e,
    reg_h,
    reg_l,
    reg_af,
    reg_hl,
    reg_bc,
    reg_de,
    reg_pc,
    reg_sp,
    reg_immediate_u8,
    reg_immediate_u16,
    reg_none
} reg_set;

typedef enum  {
    ct_none, ct_nz, ct_z, ct_nc, ct_c
} condition_type;

typedef struct {
    mnemonic  inst_mnemonic;
    addr_mode address_mode;
    reg_set op01;
    reg_set op02;
    char flags[4];
    uint cycles;
    condition_type cond_type;
    u8 param;
} instruction;

instruction get_instruction(u8 opcode);
const char* get_mnemonic_string(mnemonic str);
const char* get_addr_mode_string(addr_mode str);
const char* get_op_string(reg_set str);
const char* get_cond_type_string(condition_type str);

#endif
