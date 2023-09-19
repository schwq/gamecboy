#include "../include/instruction.h"

const char* get_mnemonic_string(mnemonic str) {
    switch(str) {
        case in_ld:
            return "in_ld";
        case in_ldh:
            return "in_ldh";
        case in_push:
            return "in_push";
        case in_pop:
            return "in_pop";
        case in_add:
            return "in_add";
        case in_adc:
            return "in_adc";
        case in_sub:
            return "in_sub";
        case in_sbc:
            return "in_sbc";
        case in_cp:
            return "in_cp";
        case in_inc:
            return "in_inc";
        case in_dec:
            return "in_dec";
        case in_and:
            return "in_and";
        case in_or:
            return "in_or";
        case in_xor:
            return "in_xor";
        case in_ccf:
            return "in_ccf";
        case in_scf:
            return "in_scf";
        case in_daa:
            return "in_daa";
        case in_cpl:
            return "in_cpl";
        case in_jp:
            return "in_jp";
        case in_jr:
            return "in_jr";
        case in_call:
            return "in_call";
        case in_ret:
            return "in_ret";
        case in_reti:
            return "in_reti";
        case in_rst:
            return "in_rst";
        case in_halt:
            return "in_halt";
        case in_stop:
            return "in_stop";
        case in_di:
            return "in_di";
        case in_ei:
            return "in_ei";
        case in_nop:
            return "in_nop";
        case in_rlca:
            return "in_rlca";
        case in_rrca:
            return "in_rrca";
        case in_rla:    
            return "in_rla";
        case in_rra:    
            return "in_rra";
        case in_cb:
            return "in_cb";
        default:
            _ERROR("get_mnemonic_string_error_unknown_str!")
            return "get_mnemonic_string_error_unknown_str!";
    }
}

const char* get_addr_mode_string(addr_mode str) {
    switch(str) {
        case am_imp:
            return "am_imp";
        case am_r_d16:
            return "am_r_d16";
        case am_r_r:
            return "am_r_r";
        case am_mr_r:
            return "am_mr_r";
        case am_r:
            return "am_r";
        case am_r_d8:
            return "am_r_d8";
        case am_r_mr:
            return "am_r_mr";
        case am_r_hli:
            return "am_r_hli";
        case am_r_hld:
            return "am_r_hld";
        case am_hli_r:
            return "am_hli_r";
        case am_hld_r:
            return "am_hld_r";
        case am_r_a8:
            return "am_r_a8";
        case am_a8_r:
            return "am_a8_r";
        case am_hl_spr:
            return "am_hl_spr";
        case am_d16:
            return "am_d16";
        case am_d8:
            return "am_d8";
        case am_d16_r:
            return "am_d16_r";
        case am_mr_d8:
            return "am_mr_d8";
        case am_mr:
            return "am_mr";
        case am_a16_r:
            return "am_a16_r";
        case am_r_a16:
            return "am_r_a16";
        case am_none:
            return "am_none";
        default:
             _ERROR("get_addr_mode_string_error_unknown_str!")
            return "get_addr_mode_string_error_unknown_str!";
           
    }
}

const char* get_op_string(reg_set str) {
    switch(str) {
        case reg_a:
            return "reg_a";
        case reg_f:
            return "reg_f";
        case reg_b:
            return "reg_b"; 
        case reg_c:
            return "reg_c";
        case reg_d:
            return "reg_d";
        case reg_e:
            return "reg_e";
        case reg_h:
            return "reg_h";
        case reg_l:
            return "reg_l";
        case reg_af:
            return "reg_af";
        case reg_hl:
            return "reg_hl";
        case reg_bc:
            return "reg_bc";
        case reg_de:
            return "reg_de";
        case reg_pc:
            return "reg_pc";
        case reg_sp:
            return "reg_sp";
        case reg_immediate_u8:
            return "reg_u8";
        case reg_immediate_u16:
            return "reg_u16";
        case reg_none:
            return "reg_none";
        default:
            _ERROR("get_op_string_error_unknown_str!")
            return "get_op_string_error_unknown_str!";
    }
}

const char* get_cond_type_string(condition_type str);

instruction inst_by_opcode[0x100] = {
    [0X00] = {in_nop, am_imp, reg_none, reg_none, NO_FLAGS, 1},
    [0X01] = {in_ld, am_r_d16, reg_bc, reg_none, NO_FLAGS, 3},
    [0X02] = {in_ld, am_mr_r, reg_bc, reg_a, NO_FLAGS, 2},
    [0X03] = {in_inc, am_r, reg_bc, reg_none, NO_FLAGS, 2},
    [0X04] = {in_inc, am_r, reg_b, reg_none, "z0h-", 1},
    [0X05] = {in_dec, am_r, reg_b, reg_none, "z1h-", 1},
    [0X06] = {in_ld, am_r_d8, reg_b, reg_none, NO_FLAGS, 2},
    [0X07] = {in_rlca, am_none, reg_none, reg_none, "000c", 1},
    [0X08] = {in_ld, am_a16_r, reg_none, reg_sp, NO_FLAGS, 5},
    [0X09] = {in_add, am_r_r, reg_hl, reg_bc, "-0hc", 2},
    [0X0A] = {in_ld, am_r_mr, reg_a, reg_bc, NO_FLAGS, 2},
    [0X0B] = {in_dec, am_r, reg_bc, reg_none, NO_FLAGS, 2},
    [0X0C] = {in_inc, am_r, reg_c, reg_none, "z0h-", 1},
    [0X0D] = {in_dec, am_r, reg_c, reg_none, "z1h-", 1},
    [0X0E] = {in_ld, am_r_d8, reg_c, reg_none, NO_FLAGS, 2},
    [0X0F] = {in_rrca, am_none, reg_none, reg_none, "000c", 1},

    //0x1x
    [0X10] = {in_stop, am_none, reg_none, reg_none, NO_FLAGS, 1},
    [0X11] = {in_ld, am_r_d16, reg_de, reg_none, NO_FLAGS, 3},
    [0X12] = {in_ld, am_mr_r, reg_de, reg_a, NO_FLAGS, 2},
    [0X13] = {in_inc, am_r, reg_de, reg_none, NO_FLAGS, 2},
    [0X14] = {in_inc, am_r, reg_d, reg_none, "z0h-", 1},
    [0X15] = {in_dec, am_r, reg_d, reg_none, "z1h-", 1},
    [0X16] = {in_ld, am_r_d8, reg_d, reg_none, NO_FLAGS, 2},
    [0X17] = {in_rla, am_none, reg_none, reg_none, "000c", 1},
    [0X18] = {in_jr, am_d8, reg_none, reg_none, NO_FLAGS, 3},
    [0X19] = {in_add, am_r_r, reg_hl, reg_de, "-0hc", 2},
    [0X1A] = {in_ld, am_r_mr, reg_a, reg_de, NO_FLAGS, 2},
    [0X1B] = {in_dec, am_r, reg_de, reg_none, NO_FLAGS, 2},
    [0X1C] = {in_inc, am_r, reg_e, reg_none, "z0h-", 1},
    [0X1D] = {in_dec, am_r, reg_e, reg_none, "z1h-", 1},
    [0X1E] = {in_ld, am_r_d8, reg_e, reg_none, NO_FLAGS, 2},
    [0X1F] = {in_rra, am_none, reg_none, reg_none, "000c", 1},

    //0x2x
    [0X20] = {in_jr, am_d8, reg_none, reg_none, NO_FLAGS, 2, ct_nz},
    [0X21] = {in_ld, am_r_d16, reg_hl, reg_none, NO_FLAGS, 3},
    [0X22] = {in_ld, am_hli_r, reg_hl, reg_a, NO_FLAGS, 2},
    [0X23] = {in_inc, am_r, reg_hl, reg_none, NO_FLAGS, 2},
    [0X24] = {in_inc, am_r, reg_h, reg_none, "z0h-", 1},
    [0X25] = {in_dec, am_r, reg_h, reg_none, "z1h-", 1},
    [0X26] = {in_ld, am_r_d8, reg_h, reg_none, NO_FLAGS, 2},
    [0X27] = {in_daa, am_none, reg_none, reg_none, "z-0c", 1},
    [0X28] = {in_jr, am_d8, reg_none, reg_none, NO_FLAGS, 2, ct_z},
    [0X29] = {in_add, am_r_r, reg_hl, reg_hl, "-0hc", 2},
    [0X2A] = {in_ld, am_r_hli, reg_a, reg_hl, NO_FLAGS, 2},
    [0X2B] = {in_dec, am_r, reg_hl, reg_none, NO_FLAGS, 2},
    [0X2C] = {in_inc, am_r, reg_l, reg_none, "z0h-", 1},
    [0X2D] = {in_dec, am_r, reg_l, reg_none, "z1h-", 1},
    [0X2E] = {in_ld, am_r_d8, reg_l, reg_none, NO_FLAGS, 2},
    [0X2F] = {in_cpl, am_none, reg_none, reg_none, "-11-", 1},

    //0x3x
    [0X30] = {in_jr, am_d8, reg_none, reg_none, NO_FLAGS, 2, ct_nc},
    [0X31] = {in_ld, am_r_d16, reg_sp, reg_none, NO_FLAGS, 2},
    [0X32] = {in_ld, am_hld_r, reg_hl, reg_a, NO_FLAGS, 2},
    [0X33] = {in_inc, am_r, reg_sp, reg_none, NO_FLAGS, 2},
    [0X34] = {in_inc, am_mr, reg_hl, reg_none, "z0h-", 3},
    [0X35] = {in_dec, am_mr, reg_hl, reg_none, "z1h-", 3},
    [0X36] = {in_ld, am_mr_d8, reg_hl, reg_none, NO_FLAGS, 3},
    [0X37] = {in_scf, am_none, reg_none, reg_none, "-001", 1},
    [0X38] = {in_jr, am_d8, reg_none, reg_none, NO_FLAGS, 2 ,ct_c},
    [0X39] = {in_add, am_r_r, reg_hl, reg_sp, "-0hc", 2},
    [0X3A] = {in_ld, am_r_hld, reg_a, reg_hl, NO_FLAGS, 2},
    [0X3B] = {in_dec, am_r, reg_sp, reg_none, NO_FLAGS, 2},
    [0X3C] = {in_inc, am_r, reg_a, reg_none, "z0h-", 1},
    [0X3D] = {in_dec, am_r, reg_a, reg_none, "z1h-", 1},
    [0X3E] = {in_ld, am_r_d8, reg_a, reg_none, NO_FLAGS, 2},
    [0X3F] = {in_ccf, am_none, reg_none, reg_none, "-00c", 1},

    //0x4x
    [0X40] = {in_ld, am_r_r, reg_b, reg_b, NO_FLAGS, 1},
    [0X41] = {in_ld, am_r_r, reg_b, reg_c, NO_FLAGS, 1},
    [0X42] = {in_ld, am_r_r, reg_b, reg_d, NO_FLAGS, 1},
    [0X43] = {in_ld, am_r_r, reg_b, reg_e, NO_FLAGS, 1},
    [0X44] = {in_ld, am_r_r, reg_b, reg_h, NO_FLAGS, 1},
    [0X45] = {in_ld, am_r_r, reg_b, reg_l, NO_FLAGS, 1},
    [0X46] = {in_ld, am_r_mr, reg_b, reg_hl, NO_FLAGS, 2},
    [0X47] = {in_ld, am_r_r, reg_b, reg_a, NO_FLAGS, 1},
    [0X48] = {in_ld, am_r_r, reg_c, reg_b, NO_FLAGS, 1},
    [0X49] = {in_ld, am_r_r, reg_c, reg_c, NO_FLAGS, 1},
    [0X4A] = {in_ld, am_r_r, reg_c, reg_d, NO_FLAGS, 1},
    [0X4B] = {in_ld, am_r_r, reg_c, reg_e, NO_FLAGS, 1},
    [0X4C] = {in_ld, am_r_r, reg_c, reg_h, NO_FLAGS, 1},
    [0X4D] = {in_ld, am_r_r, reg_c, reg_l, NO_FLAGS, 1},
    [0X4E] = {in_ld, am_r_mr, reg_c, reg_hl, NO_FLAGS, 2},
    [0X4F] = {in_ld, am_r_r, reg_c, reg_a, NO_FLAGS, 1},

    //0x5x
    [0X50] = {in_ld, am_r_r,  reg_d, reg_b, NO_FLAGS, 1},
    [0X51] = {in_ld, am_r_r,  reg_d, reg_c, NO_FLAGS, 1},
    [0X52] = {in_ld, am_r_r,  reg_d, reg_d, NO_FLAGS, 1},
    [0X53] = {in_ld, am_r_r,  reg_d, reg_e, NO_FLAGS, 1},
    [0X54] = {in_ld, am_r_r,  reg_d, reg_h, NO_FLAGS, 1},
    [0X55] = {in_ld, am_r_r,  reg_d, reg_l, NO_FLAGS, 1},
    [0X56] = {in_ld, am_r_mr, reg_d, reg_hl, NO_FLAGS, 2},
    [0X57] = {in_ld, am_r_r,  reg_d, reg_a, NO_FLAGS, 1},
    [0X58] = {in_ld, am_r_r,  reg_e, reg_b, NO_FLAGS, 1},
    [0X59] = {in_ld, am_r_r,  reg_e, reg_c, NO_FLAGS, 1},
    [0X5A] = {in_ld, am_r_r,  reg_e, reg_d, NO_FLAGS, 1},
    [0X5B] = {in_ld, am_r_r,  reg_e, reg_e, NO_FLAGS, 1},
    [0X5C] = {in_ld, am_r_r,  reg_e, reg_h, NO_FLAGS, 1},
    [0X5D] = {in_ld, am_r_r,  reg_e, reg_l, NO_FLAGS, 1},
    [0X5E] = {in_ld, am_r_mr, reg_e, reg_hl, NO_FLAGS, 2},
    [0X5F] = {in_ld, am_r_r,  reg_e, reg_a, NO_FLAGS, 1},

    //0x6x
    [0X60] = {in_ld, am_r_r,  reg_h, reg_b, NO_FLAGS, 1},
    [0X61] = {in_ld, am_r_r,  reg_h, reg_c, NO_FLAGS, 1},
    [0X62] = {in_ld, am_r_r,  reg_h, reg_d, NO_FLAGS, 1},
    [0X63] = {in_ld, am_r_r,  reg_h, reg_e, NO_FLAGS, 1},
    [0X64] = {in_ld, am_r_r,  reg_h, reg_h, NO_FLAGS, 1},
    [0X65] = {in_ld, am_r_r,  reg_h, reg_l, NO_FLAGS, 1},
    [0X66] = {in_ld, am_r_mr, reg_h, reg_hl, NO_FLAGS, 2},
    [0X67] = {in_ld, am_r_r,  reg_h, reg_a, NO_FLAGS, 1},
    [0X68] = {in_ld, am_r_r,  reg_l, reg_b, NO_FLAGS, 1},
    [0X69] = {in_ld, am_r_r,  reg_l, reg_c, NO_FLAGS, 1},
    [0X6A] = {in_ld, am_r_r,  reg_l, reg_d, NO_FLAGS, 1},
    [0X6B] = {in_ld, am_r_r,  reg_l, reg_e, NO_FLAGS, 1},
    [0X6C] = {in_ld, am_r_r,  reg_l, reg_h, NO_FLAGS, 1},
    [0X6D] = {in_ld, am_r_r,  reg_l, reg_l, NO_FLAGS, 1},
    [0X6E] = {in_ld, am_r_mr, reg_l, reg_hl, NO_FLAGS, 2},
    [0X6F] = {in_ld, am_r_r,  reg_l, reg_a, NO_FLAGS, 1},

    //0x7x
    [0X70] = {in_ld, am_mr_r,  reg_hl, reg_b, NO_FLAGS, 2},
    [0X71] = {in_ld, am_mr_r,  reg_hl, reg_c, NO_FLAGS, 2},
    [0X72] = {in_ld, am_mr_r,  reg_hl, reg_d, NO_FLAGS, 2},
    [0X73] = {in_ld, am_mr_r,  reg_hl, reg_e, NO_FLAGS, 2},
    [0X74] = {in_ld, am_mr_r,  reg_hl, reg_h, NO_FLAGS, 2},
    [0X75] = {in_ld, am_mr_r,  reg_hl, reg_l, NO_FLAGS, 2},
    [0X76] = {in_halt, am_none, reg_none, reg_none, NO_FLAGS, 1},
    [0X77] = {in_ld, am_mr_r,  reg_hl, reg_a, NO_FLAGS, 2},
    [0X78] = {in_ld, am_r_r,  reg_a, reg_b, NO_FLAGS, 1},
    [0X79] = {in_ld, am_r_r,  reg_a, reg_c, NO_FLAGS, 1},
    [0X7A] = {in_ld, am_r_r,  reg_a, reg_d, NO_FLAGS, 1},
    [0X7B] = {in_ld, am_r_r,  reg_a, reg_e, NO_FLAGS, 1},
    [0X7C] = {in_ld, am_r_r,  reg_a, reg_h, NO_FLAGS, 1},
    [0X7D] = {in_ld, am_r_r,  reg_a, reg_l, NO_FLAGS, 1},
    [0X7E] = {in_ld, am_r_mr, reg_a, reg_hl, NO_FLAGS, 2},
    [0X7F] = {in_ld, am_r_r,  reg_a, reg_a, NO_FLAGS, 1},

    //0x8x
    [0X80] = {in_add, am_r_r, reg_a, reg_b, "z0hc", 1},
    [0X81] = {in_add, am_r_r, reg_a, reg_c, "z0hc", 1},
    [0X82] = {in_add, am_r_r, reg_a, reg_d, "z0hc", 1},
    [0X83] = {in_add, am_r_r, reg_a, reg_e, "z0hc", 1},
    [0X84] = {in_add, am_r_r, reg_a, reg_h, "z0hc", 1},
    [0X85] = {in_add, am_r_r, reg_a, reg_l, "z0hc", 1},
    [0X86] = {in_add, am_r_mr, reg_a, reg_hl, "z0hc", 2},
    [0X87] = {in_add, am_r_r, reg_a, reg_a, "z0hc", 1},
    [0X88] = {in_adc, am_r_r, reg_a, reg_b, "z0hc", 1},
    [0X89] = {in_adc, am_r_r, reg_a, reg_c, "z0hc", 1},
    [0X8A] = {in_adc, am_r_r, reg_a, reg_d, "z0hc", 1},
    [0X8B] = {in_adc, am_r_r, reg_a, reg_e, "z0hc", 1},
    [0X8C] = {in_adc, am_r_r, reg_a, reg_h, "z0hc", 1},
    [0X8D] = {in_adc, am_r_r, reg_a, reg_l, "z0hc", 1},
    [0X8E] = {in_adc, am_r_mr, reg_a, reg_hl, "z0hc", 2},
    [0X8F] = {in_adc, am_r_r, reg_a, reg_a, "z0hc", 1},

    //0x9x
    [0X90] = {in_sub, am_r_r, reg_a, reg_b, "z1hc", 1},
    [0X91] = {in_sub, am_r_r, reg_a, reg_c, "z1hc", 1},
    [0X92] = {in_sub, am_r_r, reg_a, reg_d, "z1hc", 1},
    [0X93] = {in_sub, am_r_r, reg_a, reg_e, "z1hc", 1},
    [0X94] = {in_sub, am_r_r, reg_a, reg_h, "z1hc", 1},
    [0X95] = {in_sub, am_r_r, reg_a, reg_l, "z1hc", 1},
    [0X96] = {in_sub, am_r_mr, reg_a, reg_hl, "z1hc", 2},
    [0X97] = {in_sub, am_r_r, reg_a, reg_a, "z1hc", 1},
    [0X98] = {in_sbc, am_r_r, reg_a, reg_b, "z1hc", 1},
    [0X99] = {in_sbc, am_r_r, reg_a, reg_c, "z1hc", 1},
    [0X9A] = {in_sbc, am_r_r, reg_a, reg_d, "z1hc", 1},
    [0X9B] = {in_sbc, am_r_r, reg_a, reg_e, "z1hc", 1},
    [0X9C] = {in_sbc, am_r_r, reg_a, reg_h, "z1hc", 1},
    [0X9D] = {in_sbc, am_r_r, reg_a, reg_l, "z1hc", 1},
    [0X9E] = {in_sbc, am_r_mr, reg_a, reg_hl, "z1hc", 2},
    [0X9F] = {in_sbc, am_r_r, reg_a, reg_a, "z1hc", 1},


    //0xax
    [0XA0] = {in_and, am_r_r, reg_a, reg_b, "z010", 1},
    [0XA1] = {in_and, am_r_r, reg_a, reg_c, "z010", 1},
    [0XA2] = {in_and, am_r_r, reg_a, reg_d, "z010", 1},
    [0XA3] = {in_and, am_r_r, reg_a, reg_e, "z010", 1},
    [0XA4] = {in_and, am_r_r, reg_a, reg_h, "z010", 1},
    [0XA5] = {in_and, am_r_r, reg_a, reg_l, "z010", 1},
    [0XA6] = {in_and, am_r_mr, reg_a, reg_hl, "z010", 2},
    [0XA7] = {in_and, am_r_r, reg_a, reg_a, "z010", 1},
    [0XA8] = {in_xor, am_r_r, reg_a, reg_b, "z000", 1},
    [0XA9] = {in_xor, am_r_r, reg_a, reg_c, "z000", 1},
    [0XAA] = {in_xor, am_r_r, reg_a, reg_d, "z000", 1},
    [0XAB] = {in_xor, am_r_r, reg_a, reg_e, "z000", 1},
    [0XAC] = {in_xor, am_r_r, reg_a, reg_h, "z000", 1},
    [0XAD] = {in_xor, am_r_r, reg_a, reg_l, "z000", 1},
    [0XAE] = {in_xor, am_r_mr, reg_a, reg_hl, "z000", 2},
    [0XAF] = {in_xor, am_r_r, reg_a, reg_a, "z000", 1},

    //0xbx
    [0XB0] = {in_or, am_r_r, reg_a, reg_b, "z000", 1},
    [0XB1] = {in_or, am_r_r, reg_a, reg_c, "z000", 1},
    [0XB2] = {in_or, am_r_r, reg_a, reg_d, "z000", 1},
    [0XB3] = {in_or, am_r_r, reg_a, reg_e, "z000", 1},
    [0XB4] = {in_or, am_r_r, reg_a, reg_h, "z000", 1},
    [0XB5] = {in_or, am_r_r, reg_a, reg_l, "z000", 1},
    [0XB6] = {in_or, am_r_mr, reg_a, reg_hl, "z000", 2},
    [0XB7] = {in_or, am_r_r, reg_a, reg_a, "z000", 1},
    [0XB8] = {in_cp, am_r_r, reg_a, reg_b, "z1hc", 1},
    [0XB9] = {in_cp, am_r_r, reg_a, reg_c, "z1hc", 1},
    [0XBA] = {in_cp, am_r_r, reg_a, reg_d, "z1hc", 1},
    [0XBB] = {in_cp, am_r_r, reg_a, reg_e, "z1hc", 1},
    [0XBC] = {in_cp, am_r_r, reg_a, reg_h, "z1hc", 1},
    [0XBD] = {in_cp, am_r_r, reg_a, reg_l, "z1hc", 1},
    [0XBE] = {in_cp, am_r_mr, reg_a, reg_hl, "z1hc", 2},
    [0XBF] = {in_cp, am_r_r, reg_a, reg_a, "z1hc", 1},

    [0XC0] = {in_ret, am_imp, reg_none, reg_none, NO_FLAGS, 2, ct_nz},
    [0XC1] = {in_pop, am_r, reg_bc, reg_none, NO_FLAGS, 3},
    [0XC2] = {in_jp, am_d16, reg_none, reg_none, NO_FLAGS, 3 ,ct_nz},
    [0XC3] = {in_jp, am_d16, reg_none, reg_none, NO_FLAGS, 4},
    [0XC4] = {in_call, am_d16, reg_none, reg_none, NO_FLAGS, 3 ,ct_nz},
    [0XC5] = {in_push, am_r, reg_bc, reg_none, NO_FLAGS, 4},
    [0XC6] = {in_add, am_r_d8, reg_a, reg_none, "z0hc", 2},
    [0XC7] = {in_rst, am_imp, reg_none, reg_none, NO_FLAGS, 4 ,ct_none, 0x00},
    [0XC8] = {in_ret, am_imp, reg_none, reg_none, NO_FLAGS, 2 ,ct_z},
    [0XC9] = {in_ret, am_none, reg_none, reg_none, NO_FLAGS, 4},
    [0XCA] = {in_jp, am_d16, reg_none, reg_none, NO_FLAGS, 3 ,ct_z},
    [0XCB] = {in_cb, am_d8, reg_none, reg_none, NO_FLAGS, 1},
    [0XCC] = {in_call, am_d16, reg_none, reg_none, NO_FLAGS, 3 ,ct_z},
    [0XCD] = {in_call, am_d16, reg_none, reg_none, NO_FLAGS, 6},
    [0XCE] = {in_adc, am_r_d8, reg_a, reg_none, "z0hc", 2},
    [0XCF] = {in_rst, am_imp, reg_none, reg_none, NO_FLAGS, 4 ,ct_none, 0x08},

    [0XD0] = {in_ret, am_imp, reg_none, reg_none, NO_FLAGS, 2 ,ct_nc},
    [0XD1] = {in_pop, am_r, reg_de, reg_none, NO_FLAGS, 3},
    [0XD2] = {in_jp, am_d16, reg_none, reg_none, NO_FLAGS, 3 ,ct_nc},
    [0XD4] = {in_call, am_d16, reg_none, reg_none, NO_FLAGS, 3 ,ct_nc},
    [0XD5] = {in_push, am_r, reg_de, reg_none, NO_FLAGS, 4},
    [0XD6] = {in_sub, am_r_d8, reg_a, reg_none, "z1hc", 2},
    [0XD7] = {in_rst, am_imp, reg_none, reg_none, NO_FLAGS, 4, ct_none, 0x10},
    [0XD8] = {in_ret, am_imp, reg_none, reg_none, NO_FLAGS, 2 ,ct_c},
    [0XD9] = {in_reti, am_none, reg_none, reg_none, NO_FLAGS, 4},
    [0XDA] = {in_jp, am_d16, reg_none, reg_none, NO_FLAGS, 3 ,ct_c},
    [0XDC] = {in_call, am_d16, reg_none, reg_none, NO_FLAGS, 3 ,ct_c},
    [0XDE] = {in_sbc, am_r_d8, reg_a, reg_none, "z1hc", 2},
    [0XDF] = {in_rst, am_imp, reg_none, reg_none, NO_FLAGS, 3 ,ct_none, 0x18},

    //0xex
    [0XE0] = {in_ldh, am_a8_r, reg_none, reg_a, NO_FLAGS, 3},
    [0XE1] = {in_pop, am_r, reg_hl, reg_none, NO_FLAGS, 3},
    [0XE2] = {in_ld, am_mr_r, reg_c, reg_a, NO_FLAGS, 2},
    [0XE5] = {in_push, am_r, reg_hl, reg_none, NO_FLAGS, 4},
    [0XE6] = {in_and, am_r_d8, reg_a, reg_none, "z010", 2},
    [0XE7] = {in_rst, am_imp, reg_none, reg_none, NO_FLAGS, 4 ,ct_none, 0x20},
    [0XE8] = {in_add, am_r_d8, reg_sp, reg_none, "00hc", 4},
    [0XE9] = {in_jp, am_r, reg_hl, reg_none, NO_FLAGS, 1},
    [0XEA] = {in_ld, am_a16_r, reg_none, reg_a, NO_FLAGS, 4},
    [0XEE] = {in_xor, am_r_d8, reg_a, reg_none, "z000", 2},
    [0XEF] = {in_rst, am_imp, reg_none, reg_none, NO_FLAGS, 4 ,ct_none, 0x28},


    //0xfx
    [0XF0] = {in_ldh, am_r_a8, reg_a, reg_none, NO_FLAGS, 3},
    [0XF1] = {in_pop, am_r, reg_af, reg_none, "znhc", 3},
    [0XF2] = {in_ld, am_r_mr, reg_a, reg_c, NO_FLAGS, 2},
    [0XF3] = {in_di, am_none, reg_none, reg_none, NO_FLAGS, 1},
    [0XF5] = {in_push, am_r, reg_af, reg_none, NO_FLAGS, 4},
    [0XF6] = {in_or, am_r_d8, reg_a, reg_none, "z000", 2},
    [0XF7] = {in_rst, am_imp, reg_none, reg_none, NO_FLAGS, 4 ,ct_none, 0x30},
    [0XF8] = {in_ld, am_hl_spr, reg_hl, reg_sp, "00hc", 3},
    [0XF9] = {in_ld, am_r_r, reg_sp, reg_hl, NO_FLAGS, 2},
    [0XFA] = {in_ld, am_r_a16, reg_a, reg_none, NO_FLAGS, 4},
    [0XFB] = {in_ei, am_none, reg_none, reg_none, NO_FLAGS, 1},
    [0XFE] = {in_cp, am_r_d8, reg_a, reg_none, "z1hc", 2},
    [0XFF] = {in_rst, am_imp, reg_none, reg_none, NO_FLAGS, 4 ,ct_none, 0x38},
};

instruction get_instruction(u8 opcode) {
    return inst_by_opcode[opcode];
}