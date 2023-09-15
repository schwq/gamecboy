#ifndef _CPU_H_
#define _CPU_H_

#include "common.h"
#include "instruction.h"
#include "cartridge.h"
#include "ram.h"

enum flags {
    fZero = 7,
    fSubtraction = 6,
    fHalf = 5, 
    fCarry = 4
};

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
void cpu_write_reg(reg_set type, u16 value);

typedef struct {
    cpu_registers reg;
    instruction* current_inst;
    u8 current_opcode;
    bool paused;
    u8 result;
    u8 carry;
} cpu_context;

static cpu_context cpu;

#define SET_ZERO_FLAG(on) BIT_SET(cpu.reg.f, 7, on);
#define SET_CARRY_FLAG(on) BIT_SET(cpu.reg.f, 4, on);
#define SET_SUB_FLAG(on) BIT_SET(cpu.reg.f, 6, on);
#define SET_HALF_FLAG(on) BIT_SET(cpu.reg.f, 5, on);
#define GET_ZERO_FLAG() BIT(cpu.reg.f, 7);
#define GET_CARRY_FLAG() BIT(cpu.reg.f, 4);
#define GET_SUB_FLAG() BIT(cpu.reg.f, 6);
#define GET_HALF_FLAG() BIT(cpu.reg.f, 5);

#define PRINTF_ERROR_PC_REG PRINTF_BINARY_PATTERN_INT8 "\n", PRINTF_BYTE_TO_BINARY_INT8(cpu.reg.pc)

extern u16 u8_to_u16(u8 lsb, u8 msb);
extern u8 lsb(u16 number);
extern u8 msb(u16 number);

static void run_cpu();
static void init_cpu();
static void cycle_cpu(uint cycles);
static void terminate_cpu();
static void fetch_cpu(instruction* inst);
static void delay_cpu(u32 ms);
static void affected_flags(const char flags[4]);
static void increment_reg(reg_set reg, uint increment);
static void decrement_reg(reg_set reg, uint decrement);
static bool cpu_check_cond(condition_type cond);

// instruction functions proc
static void LD_proc(instruction* inst);
static void ADD_proc(instruction* inst);
static void ADC_proc(instruction* inst);
static void SUB_proc(instruction* inst);
static void SBC_proc(instruction* inst);
static void CP_proc(instruction* inst);
static void INC_proc(instruction* inst);
static void DEC_proc(instruction* inst);
static void AND_proc(instruction* inst);
static void OR_proc(instruction* inst);
static void XOR_proc(instruction* inst);
static void JP_proc(instruction* inst);
static void JR_proc(instruction* inst);
static void CALL_proc(instruction* inst);
static void RET_proc(instruction* inst);
static void RETI_proc(instruction* inst);


#endif