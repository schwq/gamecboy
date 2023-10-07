#ifndef _CPU_H_
#define _CPU_H_

#include "common.h"
#include "instruction.h"
#include "cartridge.h"
#include "ram.h"
#include "cpu_cb_inst.h"

typedef enum {
    fZero = 7,
    fSubtraction = 6,
    fHalf = 5, 
    fCarry = 4
} cpu_flags;

typedef struct {
    u8 a;
    u8 f;
    u8 b;
    u8 c;
    u8 d;
    u8 e;
    u8 h;
    u8 l;
    u16 pc;
    u16 sp;
    u8 error_reg;
} cpu_registers;

u16 reverse_value(u16 num);
u16 cpu_read_reg(reg_set type);
u16* cpu_read_pointer_u16(reg_set type);
void cpu_write_reg(reg_set type, u16 value);

typedef struct {
    bool ime;
    bool halted;
    bool ime_scheduled;
    bool shutdown;
} control_state;

typedef struct {
    cpu_registers reg;
    control_state control;
    instruction current_inst;
    u8 current_opcode;
    bool paused;
    u8 result;
    u8 c_op01;
    u8 c_op02;
    uint total_cycles; // some instructions can change the number of cycles depending of condition
} cpu_context;

extern cpu_context cpu;

#define SET_ZERO_FLAG(on) BIT_SET(cpu.reg.f, 7, on);
#define SET_CARRY_FLAG(on) BIT_SET(cpu.reg.f, 4, on);
#define SET_SUB_FLAG(on) BIT_SET(cpu.reg.f, 6, on);
#define SET_HALF_FLAG(on) BIT_SET(cpu.reg.f, 5, on);
#define GET_ZERO_FLAG() BIT(cpu.reg.f, 7);
#define GET_CARRY_FLAG() BIT(cpu.reg.f, 4);
#define GET_SUB_FLAG() BIT(cpu.reg.f, 6);
#define GET_HALF_FLAG() BIT(cpu.reg.f, 5);

#define PRINTF_ERROR_PC_REG "[%2.2X]\n", cpu.reg.pc

extern u16 u8_to_u16(u8 lsb, u8 msb);
extern u8 lsb(u16 number);
extern u8 msb(u16 number);
extern u8 rotate_left(u8 reg, uint shift);
extern u8 rotate_right(u8 reg, uint shift);

void run_cpu();
void init_cpu();
void cycle_cpu(uint cycles);
void terminate_cpu();
void fetch_cpu(instruction inst);
void delay_cpu(u32 ms);
void affected_flags(const char flags[4]);
void increment_reg(reg_set reg, uint increment);
void decrement_reg(reg_set reg, uint decrement);

// special use
void set_flag(cpu_flags flag, int value);
short get_flag(cpu_flags flag);
bool cpu_check_cond(condition_type cond);

// instruction functions proc
void LD_proc(instruction inst);
void ADD_proc(instruction inst);
void ADC_proc(instruction inst);
void SUB_proc(instruction inst);
void SBC_proc(instruction inst);
void CP_proc(instruction inst);
void INC_proc(instruction inst);
void DEC_proc(instruction inst);
void AND_proc(instruction inst);
void OR_proc(instruction inst);
void XOR_proc(instruction inst);
void JP_proc(instruction inst);
void JR_proc(instruction inst);
void CALL_proc(instruction inst);
void RET_proc(instruction inst);
void RST_proc(instruction inst);

#endif