#ifndef _CPU_CB_INSTRUCTION_
#define _CPU_CB_INSTRUCTION_

#include "common.h"
#include "cpu.h"

void cb_prefix_instruction(instruction inst);

// Rotate all bits to the left
void bit_operation_RL(u8* reg, bool carry);
void bit_operation_RL_u16(u16 reg, bool carry);
// Rotate all bits to the right
void bit_operation_RR(u8* reg, bool carry);
void bit_operation_RR_u16(u16 reg, bool carry);
// Swap the lest significate bits and the most significate bits of a register
void bit_operation_swap(u8* reg);
void bit_operation_swap_u16(u16 reg);
// Test a bit for value, if its 1, then ZERO flag is set, else nothing happens
void bit_operation_test(u8* reg, int bit);
void bit_operation_test_u16(u16 reg, int bit);
// Reset a bit to 0 
void bit_operation_reset(u8* reg, int bit);
void bit_operation_reset_u16(u16 reg, int bit);
// Rotate A right through carry
void bit_operation_RRA(bool carry);
// Shift left arithmetic (b0=0)
void bit_operation_SLA(u8* reg);
void bit_operation_SLA_u16(u16 reg);
// Shift right logical (b7=0)
void bit_operation_SRL(u8* reg);
void bit_operation_SRL_u16(u16 reg);
// Shift right arithmetic (b7=b7)
void bit_operation_SRA(u8* reg);
void bit_operation_SRA_u16(u16 reg);
// Sets a bit to 1
void bit_operation_set(u8* reg, int bit);
void bit_operation_set_u16(u16 reg, int bit);

#endif