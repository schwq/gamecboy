#include "../include/cpu.h"
#include "../include/instruction.h"
#include "common.h"

cpu_context cpu;

u8 rotate_left(u8 reg, uint shift)
{
  u8 s = shift >= 0 ? shift % 8 : -((shift) % 8);
  return (reg << s) | (reg >> (8 - s));
}

u8 rotate_right(u8 reg, uint shift)
{
  u8 s = shift >= 0 ? shift % 8 : -((shift) % 8);
  return (reg >> s) | (reg << (8 - s));
}

u16 reverse_value(u16 num)
{
  return ((num & 0xFF00) >> 8) | ((num & 0x00FF) << 8);
}

void print_flags_info()
{
  const short zero = GET_ZERO_FLAG();
  const short sub = GET_SUB_FLAG();
  const short half = GET_HALF_FLAG();
  const short carry = GET_CARRY_FLAG();
  printf("Z: %u, N: %u, H: %u, C: %u\n", zero, sub, half, carry);
}

short get_flag(cpu_flags flag)
{
  switch (flag) {
    case fZero:
      return GET_ZERO_FLAG() case fHalf : return GET_HALF_FLAG() case fCarry
          : return GET_CARRY_FLAG() case fSubtraction
          : return GET_SUB_FLAG() default
          : _ERROR("cpu_get_flag_invalid_flag_requested! returning 0!");
      return 0;
  }
}

void set_flag(cpu_flags flag, int value)
{
  switch (flag) {
    case fZero:
      SET_ZERO_FLAG(value);
      break;
    case fHalf:
      SET_HALF_FLAG(value);
      break;
    case fCarry:
      SET_CARRY_FLAG(value);
      break;
    case fSubtraction:
      SET_SUB_FLAG(value);
      break;
    default:
      _ERROR("cpu_set_flag_invalid_flag_enum!");
  }
}

void affected_flags(const char flags[4])
{
  // TODO check if this flag is doing the right job at reading
  if (flags == NO_FLAGS)
    return;
  switch (flags[0]) {
    case 'z':
      cpu.result == 0 ? set_flag(fZero, 1) : set_flag(fZero, 0);
      break;
    case '0':
      set_flag(fZero, 0);
      break;
    case '1':
      set_flag(fZero, 1);
      break;
    case '-':
      break;
    default:
      _ERROR("cpu_affected_flags_zero_flag_char_invalid!");
  }

  switch (flags[1]) {
    case '0':
      set_flag(fSubtraction, 0);
      break;
    case '1':
      set_flag(fSubtraction, 1);
      break;
    case '-':
      break;
    default:
      _ERROR("cpu_affected_flags_sub_flag_char_invalid!");
  }

  switch (flags[2]) {
    case 'h':
      // carry bit in the 3 4 bit
      (((cpu.c_op01 & 0xf) + (cpu.c_op02 & 0xf)) & 0x10) == 0x10
          ? set_flag(fHalf, 1)
          : set_flag(fHalf, 0);
      break;
    case '0':
      set_flag(fHalf, 0);
      break;
    case '1':
      set_flag(fHalf, 1);
      break;
    case '-':
      break;
    default:
      _ERROR("cpu_affected_flags_half_flag_char_invalid!");
  }

  switch (flags[3]) {
    case 'c':
      // carry bit in the 7 bit
      ((cpu.c_op01 + cpu.c_op02) > 0xff) ? set_flag(fCarry, 1)
                                         : set_flag(fCarry, 0);
      break;
    case '0':
      set_flag(fCarry, 0);
      break;
    case '1':
      set_flag(fCarry, 1);
      break;
    case '-':
      break;
    default:
      _ERROR("cpu_affected_flags_carry_flag_char_invalid!");
  }
}

void increment_reg(reg_set reg, uint increment)
{
  cpu_write_reg(reg, cpu_read_reg(reg) + increment);
}

void decrement_reg(reg_set reg, uint decrement)
{
  cpu_write_reg(reg, cpu_read_reg(reg) - decrement);
}

bool cpu_check_cond(condition_type cond)
{
  if (cond == ct_z) {
    return (bool)GET_ZERO_FLAG()
  }
  if (cond == ct_c) {
    return (bool)GET_CARRY_FLAG()
  }
  if (cond == ct_nc) {
    bool carry = GET_CARRY_FLAG() return !carry;
  }
  if (cond == ct_nz) {
    bool zero = GET_ZERO_FLAG() return !zero;
  }
  return true;
}

u16 cpu_read_reg(reg_set type)
{
  switch (type) {
    case reg_a:
      return cpu.reg.a;
    case reg_b:
      return cpu.reg.b;
    case reg_c:
      return cpu.reg.c;
    case reg_d:
      return cpu.reg.d;
    case reg_e:
      return cpu.reg.e;
    case reg_f:
      return cpu.reg.f;
    case reg_h:
      return cpu.reg.h;
    case reg_l:
      return cpu.reg.l;
    case reg_immediate_u8:
      return read_uint8_data(++cpu.reg.pc);

    case reg_af:
      return reverse_value(*((u16*)&cpu.reg.a));
    case reg_bc:
      return reverse_value(*((u16*)&cpu.reg.b));
    case reg_de:
      return reverse_value(*((u16*)&cpu.reg.d));
    case reg_hl:
      return reverse_value(*((u16*)&cpu.reg.h));

    case reg_sp:
      return cpu.reg.sp;
    case reg_pc:
      return cpu.reg.pc;
    //case reg_immediate_u16: return read_uint16_data(cpu.reg.pc++);
    case reg_none:
    default:
      printf("cpu_read_reg_error, returning 0!\n");
      return 0;
  }
}

u16 u8_to_u16(u8 lsb, u8 msb)
{
  u16 temp = ((u16)msb << 8) | (u16)lsb;
  return temp;
}

u8 lsb(u16 number)
{
  u8 temp = number & 0x00FF;
  return temp;
}

u8 msb(u16 number)
{
  u8 temp = number >> 8;
  return temp;
}

void cpu_write_reg(reg_set type, u16 value)
{
  switch (type) {
    case reg_a:
      cpu.reg.a = value & 0xFF;
      break;
    case reg_b:
      cpu.reg.b = value & 0xFF;
      break;
    case reg_c:
      cpu.reg.c = value & 0xFF;
      break;
    case reg_d:
      cpu.reg.d = value & 0xFF;
      break;
    case reg_e:
      cpu.reg.e = value & 0xFF;
      break;
    case reg_f:
      cpu.reg.f = value & 0xFF;
      break;
    case reg_h:
      cpu.reg.h = value & 0xFF;
      break;
    case reg_l:
      cpu.reg.l = value & 0xFF;
      break;

    case reg_af:
      *((u16*)&cpu.reg.a) = reverse_value(value);
      break;
    case reg_bc:
      *((u16*)&cpu.reg.b) = reverse_value(value);
      break;
    case reg_de:
      *((u16*)&cpu.reg.d) = reverse_value(value);
      break;
    case reg_hl:
      *((u16*)&cpu.reg.h) = reverse_value(value);
      break;

    case reg_sp:
      cpu.reg.sp = value;
      break;
    case reg_pc:
      cpu.reg.pc = value;
      break;
    case reg_none:
    default:
      _ERROR("cpu_write_register_unknown_register_error!");
  }
}

void run_cpu()
{
  cpu.pc_can_increment = true;
  instruction inst = get_instruction(read_uint8_data(cpu.reg.pc));

  cpu.current_opcode = read_uint8_data(cpu.reg.pc);

  printf(
      "[INST]: %s [OP01]: %s [OP01]: %s [ADDR]: %s [OPCODE]: 0x%2.2X [PC]: "
      "0x%2.2X ",
      get_mnemonic_string(inst.inst_mnemonic),\ 
            get_op_string(inst.op01),
      get_op_string(inst.op02), get_addr_mode_string(inst.address_mode),
      cpu.current_opcode, cpu.reg.pc);
  print_flags_info();
  printf("reg b " HEX_PATTERN ", reg c " HEX_PATTERN " \n", cpu_read_reg(reg_b),
         cpu_read_reg(reg_c));
  if (DG_STOP_AT_RST_IN) {
    if (inst.inst_mnemonic == in_rst) {
      cpu.control.shutdown = true;
    }
  }

  fetch_cpu(inst);

  if (!cpu.control.shutdown) {
    cpu.last_zero_flag = GET_ZERO_FLAG();
    cpu.last_carry_flag = GET_CARRY_FLAG();
    affected_flags(inst.flags);
    cycle_cpu(cpu.total_cycles);
    if (cpu.pc_can_increment)
      cpu.reg.pc++;
  }
}

void init_cpu()
{
  cpu.reg.pc = 0x0100;
  cpu.reg.sp = 0xFFFE;
  cpu_write_reg(reg_af, 0x01B0);
  cpu_write_reg(reg_bc, 0x0013);
  cpu_write_reg(reg_de, 0x00D8);
  cpu_write_reg(reg_hl, 0x014D);
  cpu.control.ime = false;
  cpu.control.ime_scheduled = false;
  cpu.control.halted = false;
  cpu.paused = false;
  cpu.control.shutdown = false;
  cpu.last_zero_flag = 0;
  cpu.last_carry_flag = 0;
}

void cycle_cpu(uint cycles)
{
  // TODO
}

void fetch_cpu(instruction inst)
{
  cpu.c_op01 = 0;
  cpu.c_op02 = 0;
  cpu.result = 0;
  cpu.total_cycles = inst.cycles;

  switch (inst.inst_mnemonic) {
    case in_ld:
    case in_ldh:
      LD_proc(inst);
      break;
    case in_nop:
      break;
    case in_push:
      cpu.reg.sp--;
      write_uint8_data(cpu.reg.sp--, msb(cpu_read_reg(inst.op01)));
      write_uint8_data(cpu.reg.sp, lsb(cpu_read_reg(inst.op01)));
      break;
    case in_pop: {
      u16 addr1 = cpu.reg.sp++;
      u16 addr2 = cpu.reg.sp++;
      cpu_write_reg(inst.op01,
                    u8_to_u16(read_uint8_data(addr1), read_uint8_data(addr2)));
    } break;
    case in_add:
      ADD_proc(inst);
      break;
    case in_adc:
      ADC_proc(inst);
      break;
    case in_sub:
      SUB_proc(inst);
      break;
    case in_sbc:
      SBC_proc(inst);
      break;
    case in_and:
      AND_proc(inst);
      break;
    case in_or:
      OR_proc(inst);
      break;
    case in_xor:
      XOR_proc(inst);
      break;
    case in_jp:
      JP_proc(inst);
      break;
    case in_jr:
      JR_proc(inst);
      break;
    case in_dec:
      DEC_proc(inst);
      break;
    case in_inc:
      INC_proc(inst);
      break;
    case in_cp:
      CP_proc(inst);
      break;
    case in_ccf: {
      SET_HALF_FLAG(0)
      SET_SUB_FLAG(0)
      short data = GET_CARRY_FLAG() SET_CARRY_FLAG(~data)
    } break;
    case in_scf:
      SET_CARRY_FLAG(1)
      SET_HALF_FLAG(0)
      SET_SUB_FLAG(0)
      break;
    case in_daa:
      // TODO: https://ehaskins.com/2018-01-30 Z80 DAA/
      break;
    case in_cpl:
      cpu.reg.a = ~cpu.reg.a;
      SET_SUB_FLAG(1)
      SET_HALF_FLAG(1)
      break;
    case in_call:
      CALL_proc(inst);
      break;
    case in_ret:
      RET_proc(inst);
      break;
    case in_reti: {
      u16 addr1 = cpu.reg.sp++;
      u16 addr2 = cpu.reg.sp++;
      cpu.reg.pc = u8_to_u16(read_uint8_data(addr1), read_uint8_data(addr2));
      cpu.control.ime = 1;
    } break;
    case in_rst:
      RST_proc(inst);
      break;
    case in_di:
      cpu.control.ime = 0;
      break;
    case in_ei:
      cpu.control.ime_scheduled = true;
      break;
    case in_stop:
      // TODO
      break;
    case in_halt:
      // TODO
      cpu.control.halted = true;
      break;
    case in_cb:
      cb_prefix_instruction(inst);
      break;
    case in_rlca:
      cpu.reg.a = rotate_left(cpu.reg.a, 1);
      break;
    case in_rla: {
      short carry = GET_CARRY_FLAG() cpu.reg.a = rotate_left(cpu.reg.a, carry);
    } break;
    case in_rrca:
      cpu.reg.a = rotate_right(cpu.reg.a, 1);
      break;
    case in_rra: {
      short carry = GET_CARRY_FLAG() cpu.reg.a = rotate_right(cpu.reg.a, carry);
    } break;
    default:
      _ERROR("fetch_cpu_error, cannot find mnemonic! PC: " PRINTF_ERROR_PC_REG);
      SHUTDOWN_CPU();
      _CRITICAL;
  }
}

void LD_proc(instruction inst)
{
  switch (inst.address_mode) {
    case am_r_r:
      cpu_write_reg(inst.op01, cpu_read_reg(inst.op02));
      break;
    case am_r_d8:
      cpu_write_reg(inst.op01, read_uint8_data(++cpu.reg.pc));
      break;
    case am_hli_r:  //0x22
      write_uint8_data(cpu_read_reg(reg_hl), cpu_read_reg(inst.op02));
      increment_reg(reg_hl, 1);
      break;
    case am_r_hld:
      // LD A, (HL-): Load accumulator (indirect HL, decrement)
      cpu_write_reg(reg_a, read_uint8_data(cpu_read_reg(reg_hl)));
      decrement_reg(reg_hl, 1);
      break;
    case am_a16_r: {
      u16 addr1 = ++cpu.reg.pc;
      u16 addr2 = ++cpu.reg.pc;
      u16 nn = u8_to_u16(read_uint8_data(addr1), read_uint8_data(addr2));
      if (cpu.current_opcode == 0x08) {
        write_uint8_data(nn, lsb(cpu.reg.sp));
        write_uint8_data(nn + 1, msb(cpu.reg.sp));
      }
      else {
        write_uint8_data(nn, (u8)cpu_read_reg(inst.op02));
      }
      break;
    }
    case am_r_d16: {
      u16 addr1 = ++cpu.reg.pc;
      u16 addr2 = ++cpu.reg.pc;
      u16 nn = u8_to_u16(read_uint8_data(addr1), read_uint8_data(addr2));
      cpu_write_reg(inst.op01, nn);
      break;
    }
    case am_r_a16: {
      u16 addr1 = ++cpu.reg.pc;
      u16 addr2 = ++cpu.reg.pc;
      u16 nn = u8_to_u16(read_uint8_data(addr1), read_uint8_data(addr2));
      cpu_write_reg(inst.op01, read_uint8_data(nn));
      break;
    }
    case am_r_mr:
      // LDH A, (C): Load accumulator (indirect 0xFF00+C)
      cpu_write_reg(inst.op01,
                    read_uint8_data(u8_to_u16(cpu_read_reg(inst.op02), 0xFF)));
      break;
    case am_mr_r:
      // LDH (C), A: Load from accumulator (indirect 0xFF00+C)
      write_uint8_data(u8_to_u16(cpu_read_reg(inst.op01), 0xFF),
                       cpu_read_reg(inst.op02));
      break;
    case am_r_a8:
      // LDH A, (n): Load accumulator (direct 0xFF00+n)
      {
        u8 n = read_uint8_data(++cpu.reg.pc);
        cpu_write_reg(inst.op01, u8_to_u16(n, 0xFF));
        break;
      }
    case am_a8_r:
      // LDH (n), A: Load from accumulator (direct 0xFF00+n)
      {
        u8 n = read_uint8_data(++cpu.reg.pc);
        write_uint8_data(u8_to_u16(n, 0xFF), cpu_read_reg(inst.op02));
        break;
      }
    case am_hld_r:
      // LD (HL-), A: Load from accumulator (indirect HL, decrement)
      write_uint8_data(cpu_read_reg(reg_hl), cpu_read_reg(inst.op02));
      decrement_reg(reg_hl, 1);
      break;
    case am_r_hli:
      // LD A, (HL+): Load accumulator (indirect HL, increment)
      cpu_write_reg(inst.op01, read_uint8_data(cpu_read_reg(reg_hl)));
      increment_reg(reg_hl, 1);
      break;
    case am_mr_d8: {
      u8 nn = read_uint8_data(++cpu.reg.pc);
      write_uint8_data(cpu_read_reg(reg_hl), nn);
    } break;
    default:
      _ERROR(
          "in_ld_proc_error_invalid_address_mode, cannot load instruction "
          "PC: " PRINTF_ERROR_PC_REG);
      SHUTDOWN_CPU();
      _CRITICAL;
  }
}

void ADD_proc(instruction inst)
{
  switch (inst.address_mode) {
    case am_r_r:
      // ADD r: Add (register)
      {
        u8 data = cpu_read_reg(inst.op01) + cpu_read_reg(inst.op02);
        cpu.c_op01 = cpu_read_reg(inst.op01);
        cpu.c_op02 = cpu_read_reg(inst.op02);
        cpu_write_reg(inst.op01, data);
        cpu.result = data;
      }
      break;
    case am_r_mr:
      //ADD (HL): Add (indirect HL)
      {
        u8 data = read_uint8_data(cpu_read_reg(inst.op02));
        cpu.c_op01 = data;
        cpu.c_op02 = cpu_read_reg(inst.op01);
        data += cpu_read_reg(inst.op01);
        cpu_write_reg(inst.op01, data);
        cpu.result = data;
      }
      break;
    case am_r_d8: {
      u8 data = read_uint8_data(++cpu.reg.pc);
      cpu.c_op01 = data;
      cpu.c_op02 = cpu_read_reg(inst.op01);
      data += cpu_read_reg(inst.op01);
      cpu_write_reg(inst.op01, data);
      cpu.result = data;
    } break;
    default:
      _ERROR(
          "in_add_proc_error_invalid_address_mode,  cannot load instruction "
          "PC: " PRINTF_ERROR_PC_REG);
      SHUTDOWN_CPU();
      _CRITICAL;
  }
}

void ADC_proc(instruction inst)
{
  switch (inst.address_mode) {
    case am_r_r: {
      u8 data =
          cpu_read_reg(inst.op01) + cpu_read_reg(inst.op02) + GET_CARRY_FLAG();
      cpu.c_op01 = cpu_read_reg(inst.op01);
      cpu.c_op02 = cpu_read_reg(inst.op02) + GET_CARRY_FLAG()
                                                 cpu_write_reg(inst.op01, data);
      cpu.result = data;
    } break;
    case am_r_mr: {
      u8 data = read_uint8_data(cpu_read_reg(inst.op02));
      cpu.c_op01 = data;
      cpu.c_op02 = cpu_read_reg(inst.op01) + GET_CARRY_FLAG();
      data += cpu_read_reg(inst.op01) + GET_CARRY_FLAG();
      cpu_write_reg(inst.op01, data);
      cpu.result = data;
    } break;
    case am_r_d8: {
      u8 data = read_uint8_data(++cpu.reg.pc);
      cpu.c_op01 = data;
      cpu.c_op02 = cpu_read_reg(inst.op01) + GET_CARRY_FLAG();
      data += cpu_read_reg(inst.op01) + GET_CARRY_FLAG();
      cpu_write_reg(inst.op01, data);
      cpu.result = data;
    } break;
    default:
      _ERROR(
          "in_adc_proc_error_invalid_address_mode cannot load instruction "
          "PC: " PRINTF_ERROR_PC_REG);
      SHUTDOWN_CPU();
      _CRITICAL;
  }
}
void SUB_proc(instruction inst)
{
  switch (inst.address_mode) {
    case am_r_r: {
      u8 data = cpu_read_reg(inst.op01) - cpu_read_reg(inst.op02);
      cpu.c_op01 = cpu_read_reg(inst.op01);
      cpu.c_op02 = -(cpu_read_reg(
          inst.op02));  // TODO check if inverting the value changes the need to implement carry for subtraction
      cpu_write_reg(inst.op01, data);
      cpu.result = data;
    } break;
    case am_r_mr: {
      u8 data = read_uint8_data(cpu_read_reg(inst.op02));
      cpu.c_op01 = data;
      cpu.c_op02 = -(cpu_read_reg(inst.op01));
      data -= cpu_read_reg(inst.op01);
      cpu_write_reg(inst.op01, data);
      cpu.result = data;
    } break;
    case am_r_d8: {
      u8 data = read_uint8_data(++cpu.reg.pc);
      cpu.c_op01 = data;
      cpu.c_op02 = -(cpu_read_reg(inst.op01));
      data -= cpu_read_reg(inst.op01);
      cpu_write_reg(inst.op01, data);
      cpu.result = data;
    } break;
    default:
      _ERROR(
          "in_sub_proc_error_invalid_address_mode,  cannot load instruction "
          "PC: " PRINTF_ERROR_PC_REG);
      SHUTDOWN_CPU();
      _CRITICAL;
  }
}

void SBC_proc(instruction inst)
{
  switch (inst.address_mode) {
    case am_r_r: {
      u8 data =
          cpu_read_reg(inst.op01) - cpu_read_reg(inst.op02) - GET_CARRY_FLAG();
      cpu.c_op01 = cpu_read_reg(inst.op01);
      cpu.c_op02 = -(cpu_read_reg(inst.op02) - get_flag(fCarry));
      cpu_write_reg(inst.op01, data);
      cpu.result = data;
    } break;
    case am_r_mr: {
      u8 data = read_uint8_data(cpu_read_reg(inst.op02));
      cpu.c_op01 = read_uint8_data(cpu_read_reg(inst.op02));
      cpu.c_op02 = -(cpu_read_reg(inst.op01) - get_flag(fCarry));
      data -= cpu_read_reg(inst.op01) - GET_CARRY_FLAG();
      cpu_write_reg(inst.op01, data);
      cpu.result = data;
    } break;
    case am_r_d8: {
      u8 data = read_uint8_data(++cpu.reg.pc);
      cpu.c_op01 = read_uint8_data(++cpu.reg.pc);
      cpu.c_op02 = -(cpu_read_reg(inst.op01) - get_flag(fCarry));
      data -= cpu_read_reg(inst.op01) - GET_CARRY_FLAG();
      cpu_write_reg(inst.op01, data);
      cpu.result = data;
    } break;
    default:
      _ERROR(
          "in_sbc_proc_error_invalid_address_mode cannot load instruction "
          "PC: " PRINTF_ERROR_PC_REG);
      SHUTDOWN_CPU();
      _CRITICAL;
  }
}

void CP_proc(instruction inst)
{
  switch (inst.address_mode) {
    case am_r_r: {
      u8 data = cpu_read_reg(inst.op01) - cpu_read_reg(inst.op02);
      cpu.c_op01 = cpu_read_reg(inst.op01);
      cpu.c_op02 = -(cpu_read_reg(inst.op02));
      cpu.result = data;
    } break;
    case am_r_mr: {
      u8 data = read_uint8_data(cpu_read_reg(inst.op02));
      cpu.c_op01 = data;
      cpu.c_op02 = -(cpu_read_reg(inst.op01));
      data -= cpu_read_reg(inst.op01);
      cpu.result = data;
    } break;
    case am_r_d8: {
      u8 data = read_uint8_data(++cpu.reg.pc);
      cpu.c_op01 = data;
      cpu.c_op02 = -(cpu_read_reg(++cpu.reg.pc));
      data -= cpu_read_reg(inst.op01);
      cpu.result = data;
    } break;
    default:
      _ERROR(
          "in_cp_proc_error_invalid_address_mode cannot load instruction "
          "PC: " PRINTF_ERROR_PC_REG);
      SHUTDOWN_CPU();
      _CRITICAL;
  }
}

void INC_proc(instruction inst)
{
  switch (inst.address_mode) {
    case am_r: {
      u8 data = cpu_read_reg(inst.op01);
      cpu.c_op01 = data;
      cpu.c_op02 = 1;
      cpu_write_reg(inst.op01, data + 1);
      cpu.result = data;
    } break;
    case am_mr: {
      u8 data = read_uint8_data(cpu_read_reg(inst.op01)) + 1;
      cpu.c_op01 = read_uint8_data(cpu_read_reg(inst.op01));
      cpu.c_op02 = 1;
      write_uint8_data(cpu_read_reg(inst.op01), data);
      cpu.result = data;
    } break;
    default:
      _ERROR(
          "in_inc_proc_error_invalid_address_mode cannot load instruction "
          "PC: " PRINTF_ERROR_PC_REG);
      SHUTDOWN_CPU();
      _CRITICAL;
  }
}

void DEC_proc(instruction inst)
{
  switch (inst.address_mode) {
    case am_r: {
      u8 data = cpu_read_reg(inst.op01);
      cpu.c_op01 = data;
      cpu.c_op02 = -1;
      cpu_write_reg(inst.op01, data - 1);
      cpu.result = data;
    } break;
    case am_mr: {
      u8 data = read_uint8_data(cpu_read_reg(inst.op01)) - 1;
      cpu.c_op01 = read_uint8_data(cpu_read_reg(inst.op01));
      cpu.c_op02 = -1;
      write_uint8_data(cpu_read_reg(inst.op01), data);
      cpu.result = data;
    } break;
    default:
      _ERROR(
          "in_dec_proc_error_invalid_address_mode cannot load instruction "
          "PC: " PRINTF_ERROR_PC_REG);
      SHUTDOWN_CPU();
      _CRITICAL;
  }
}

void AND_proc(instruction inst)
{
  switch (inst.address_mode) {
    case am_r_r: {
      u8 data = cpu_read_reg(inst.op01) & cpu_read_reg(inst.op02);
      cpu_write_reg(inst.op01, data);
      cpu.result = data;
    } break;
    case am_r_mr: {
      u8 data = read_uint8_data(cpu_read_reg(inst.op02));
      data &= cpu_read_reg(inst.op01);
      cpu_write_reg(inst.op01, data);
      cpu.result = data;
    } break;
    case am_r_d8: {
      u8 data = read_uint8_data(++cpu.reg.pc);
      data &= cpu_read_reg(inst.op01);
      cpu_write_reg(inst.op01, data);
      cpu.result = data;
    } break;
    default:
      _ERROR(
          "in_and_proc_error_invalid_address_mode cannot load instruction "
          "PC: " PRINTF_ERROR_PC_REG);
      SHUTDOWN_CPU();
      _CRITICAL;
  }
}

void OR_proc(instruction inst)
{
  switch (inst.address_mode) {
    case am_r_r: {
      u8 data = cpu_read_reg(inst.op01) | cpu_read_reg(inst.op02);
      cpu_write_reg(inst.op01, data);
      cpu.result = data;
    } break;
    case am_r_mr: {
      u8 data = read_uint8_data(cpu_read_reg(inst.op02));
      data |= cpu_read_reg(inst.op01);
      cpu_write_reg(inst.op01, data);
      cpu.result = data;
    } break;
    case am_r_d8: {
      u8 data = read_uint8_data(++cpu.reg.pc);
      data |= cpu_read_reg(inst.op01);
      cpu_write_reg(inst.op01, data);
      cpu.result = data;
    } break;
    default:
      _ERROR(
          "in_or_proc_error_invalid_address_mode cannot load instruction "
          "PC: " PRINTF_ERROR_PC_REG);
      SHUTDOWN_CPU();
      _CRITICAL;
  }
}

void XOR_proc(instruction inst)
{
  switch (inst.address_mode) {
    case am_r_r: {
      u8 data = cpu_read_reg(inst.op01) ^ cpu_read_reg(inst.op02);
      cpu_write_reg(inst.op01, data);
      cpu.result = data;
    } break;
    case am_r_mr: {
      u8 data = read_uint8_data(cpu_read_reg(inst.op02));
      data ^= cpu_read_reg(inst.op01);
      cpu_write_reg(inst.op01, data);
      cpu.result = data;
    } break;
    case am_r_d8: {
      u8 data = read_uint8_data(++cpu.reg.pc);
      data ^= cpu_read_reg(inst.op01);
      cpu_write_reg(inst.op01, data);
      cpu.result = data;
    } break;
    default:
      _ERROR(
          "in_xor_proc_error_invalid_address_mode cannot load instruction "
          "PC: " PRINTF_ERROR_PC_REG);
      SHUTDOWN_CPU();
      _CRITICAL;
  }
}

void JP_proc(instruction inst)
{
  switch (inst.address_mode) {
    case am_r:
      cpu.reg.pc = cpu_read_reg(inst.op01);
      cpu.pc_can_increment = false;
      break;
    case am_d16: {
      u16 addr1 = ++cpu.reg.pc;
      u16 addr2 = ++cpu.reg.pc;
      u16 nn = u8_to_u16(read_uint8_data(addr1), read_uint8_data(addr2));
      if (cpu.current_opcode == 0xC3) {
        cpu.reg.pc = nn;
        cpu.pc_can_increment = false;
      }
      else {
        if (cpu_check_cond(inst.cond_type)) {
          cpu.reg.pc = nn;
          cpu.pc_can_increment = false;
        }
      }
    } break;
    default:
      _ERROR(
          "in_jp_proc_error_invalid_address_mode cannot load instruction "
          "PC: " PRINTF_ERROR_PC_REG);
      SHUTDOWN_CPU();
      _CRITICAL;
  }
}

void JR_proc(instruction inst)
{
  switch (inst.address_mode) {
    case am_d8: {
      int8_t nn = (int8_t)read_uint8_data(++cpu.reg.pc);
      if (cpu.current_opcode == 0x18) {
        cpu.reg.pc += nn;
      }
      else {
        printf("Condition type : %s\n", get_cond_type_string(inst.cond_type));
        if (cpu_check_cond(inst.cond_type)) {
          cpu.reg.pc += nn;
          // TODO the pc must be incremented here, for some reason
        }
      }
    } break;
    default:
      _ERROR(
          "in_jr_proc_error_invalid_address_mode cannot load instruction "
          "PC: " PRINTF_ERROR_PC_REG);
      SHUTDOWN_CPU();
      _CRITICAL;
  }
}

void CALL_proc(instruction inst)
{
  switch (inst.address_mode) {
    case am_d16: {
      u16 addr1 = ++cpu.reg.pc;
      u16 addr2 = ++cpu.reg.pc;
      u16 nn = u8_to_u16(read_uint8_data(addr1), read_uint8_data(addr2));
      if (cpu.current_opcode == 0xCD) {
        cpu.reg.sp--;
        write_uint8_data(cpu.reg.sp--, msb(cpu.reg.pc));
        write_uint8_data(cpu.reg.sp, lsb(cpu.reg.pc));
        cpu.reg.pc = nn;
      }
      else {
        if (cpu_check_cond(inst.cond_type)) {
          cpu.reg.sp--;
          write_uint8_data(cpu.reg.sp--, msb(cpu.reg.pc));
          write_uint8_data(cpu.reg.sp, lsb(cpu.reg.pc));
          cpu.reg.pc = nn;
        }
      }
    } break;
    default:
      _ERROR(
          "in_call_proc_error_invalid_address_mode cannot load instruction "
          "PC: " PRINTF_ERROR_PC_REG);
      SHUTDOWN_CPU();
      _CRITICAL;
  }
}

void RET_proc(instruction inst)
{
  switch (inst.address_mode) {
    case am_imp: {
      u16 addr1 = cpu.reg.sp++;
      u16 addr2 = cpu.reg.sp++;
      if (cpu.current_opcode == 0xC9) {
        cpu.reg.pc = u8_to_u16(read_uint8_data(addr1), read_uint8_data(addr2));
      }
      else {
        if (cpu_check_cond(inst.cond_type)) {
          cpu.reg.pc =
              u8_to_u16(read_uint8_data(addr1), read_uint8_data(addr2));
        }
      }
    } break;
    case am_none: {
      u8 data1 = read_uint8_data(cpu.reg.sp++);
      u8 data2 = read_uint8_data(cpu.reg.sp++);
      cpu.reg.pc = u8_to_u16(data1, data2);
    } break;
    default:
      _ERROR(
          "in_ret_proc_error_invalid_address_mode cannot load instruction "
          "PC: " PRINTF_ERROR_PC_REG);
      SHUTDOWN_CPU();
      _CRITICAL;
  }
}

void RST_proc(instruction inst)
{
  switch (inst.address_mode) {
    case am_imp:
      // todo check func
      cpu.reg.sp--;
      write_uint8_data(cpu.reg.sp--, msb(cpu.reg.pc));
      write_uint8_data(cpu.reg.sp, lsb(cpu.reg.pc));
      cpu.reg.pc = u8_to_u16(inst.param, 0x00);
      break;
    default:
      _ERROR(
          "in_rst_proc_error_invalid_address_mode cannot load instruction "
          "PC: " PRINTF_ERROR_PC_REG);
      SHUTDOWN_CPU();
      _CRITICAL;
  }
}
