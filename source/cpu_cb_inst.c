#include "../include/cpu_cb_inst.h"
#include "../include/cpu.h"
// Rotate all bits to the left
void bit_operation_RL(u8* reg, bool carry)
{
  u8 bit7 = (*reg & 0x80) >> 7;
  u8 value = *reg << 1;
  if (carry) {
    *reg = value + bit7;
  }
  else {
    *reg = value + GET_CARRY_FLAG();
  }

  SET_HALF_FLAG(0);
  SET_SUB_FLAG(0);
  SET_ZERO_FLAG((*reg == 0));
  SET_CARRY_FLAG((bool)bit7);
}

void bit_operation_RL_u16(u16 reg, bool carry)
{
  u8 bit7 = (read_uint8_data(reg) & 0x80) >> 7;
  u8 value = read_uint8_data(reg) << 1;
  if (carry) {
    write_uint8_data(reg, value + bit7);
  }
  else {
    short carry_bit = GET_CARRY_FLAG();
    write_uint8_data(reg, value + carry_bit);
  }

  SET_HALF_FLAG(0);
  SET_SUB_FLAG(0);
  SET_ZERO_FLAG((read_uint8_data(reg) == 0));
  SET_CARRY_FLAG((bool)bit7);
}

// Rotate all bits to the right
void bit_operation_RR(u8* reg, bool carry)
{
  u8 bit0 = *reg & 0x01;
  u8 value = *reg >> 1;

  if (carry) {
    *reg = value + (bit0 << 7);
  }
  else {
    short carry_bit = GET_CARRY_FLAG();
    *reg = value + (carry_bit << 7);
  }

  SET_HALF_FLAG(0);
  SET_SUB_FLAG(0);
  SET_ZERO_FLAG((*reg == 0));
  SET_CARRY_FLAG((bool)bit0);
}

void bit_operation_RR_u16(u16 reg, bool carry)
{
  u8 bit0 = read_uint8_data(reg) & 0x01;
  u8 value = read_uint8_data(reg) >> 1;

  if (carry) {
    write_uint8_data(reg, value + (bit0 << 7));
  }
  else {
    short carry_bit = GET_CARRY_FLAG();
    write_uint8_data(reg, value + (carry_bit << 7));
  }

  SET_HALF_FLAG(0);
  SET_SUB_FLAG(0);
  SET_ZERO_FLAG((read_uint8_data(reg) == 0));
  SET_CARRY_FLAG((bool)bit0);
}

// Swap the lest significate bits and the most significate bits of a register
void bit_operation_swap(u8* reg)
{
  *reg = (*reg >> 4) | ((*reg & 0xF) << 4);

  SET_HALF_FLAG(0);
  SET_SUB_FLAG(0);
  SET_ZERO_FLAG(*reg == 0);
  SET_CARRY_FLAG(0);
}

void bit_operation_swap_u16(u16 reg)
{
  write_uint8_data(
      reg, (read_uint8_data(reg) >> 4) | ((read_uint8_data(reg) & 0xF) << 4));

  SET_HALF_FLAG(0);
  SET_SUB_FLAG(0);
  SET_ZERO_FLAG(read_uint8_data(reg) == 0);
  SET_CARRY_FLAG(0);
}

// Test a bit for value, if its 1, then ZERO flag is set, else nothing happens
void bit_operation_test(u8* reg, int bit)
{
  u8 value = (*reg >> bit) & 1;

  SET_SUB_FLAG(0);
  SET_HALF_FLAG(0);
  SET_ZERO_FLAG(value == 0);
}

void bit_operation_test_u16(u16 reg, int bit)
{
  u8 value = (read_uint8_data(reg) >> bit) & 1;

  SET_SUB_FLAG(0);
  SET_HALF_FLAG(0);
  SET_ZERO_FLAG(value == 0);
}

// Reset a bit to 0
void bit_operation_reset(u8* reg, int bit)
{
  u8 bit_mask = 1 << bit;
  u8 value = *reg & ~bit_mask;
  *reg = value;
}

void bit_operation_reset_u16(u16 reg, int bit)
{
  u8 bit_mask = 1 << bit;
  u8 value = read_uint8_data(reg) & ~bit_mask;
  write_uint8_data(reg, value);
}

// Rotate A right through carry
void bit_operation_RRA(bool carry)
{
  u8 bit0 = cpu.reg.a & 0x01;
  u8 value = cpu.reg.a >> 1;

  if (carry) {
    cpu.reg.a = value + (bit0 << 7);
  }
  else {
    short carry_bit = GET_CARRY_FLAG();
    cpu.reg.a = value + (carry_bit << 7);
  }

  SET_HALF_FLAG(0);
  SET_SUB_FLAG(0);
  SET_ZERO_FLAG(0);
  SET_CARRY_FLAG((bool)bit0);
}

// Shift left arithmetic (b0=0)
void bit_operation_SLA(u8* reg)
{
  u8 bit7 = (*reg & 0x80) >> 7;
  *reg <<= 1;

  SET_HALF_FLAG(0);
  SET_SUB_FLAG(0);
  SET_ZERO_FLAG((*reg == 0));
  SET_CARRY_FLAG((bool)bit7);
}

void bit_operation_SLA_u16(u16 reg)
{
  u8 bit7 = (read_uint8_data(reg) & 0x80) >> 7;
  u8 value = read_uint8_data(reg);
  write_uint8_data(reg, value <<= 1);

  SET_HALF_FLAG(0);
  SET_SUB_FLAG(0);
  SET_ZERO_FLAG((read_uint8_data(reg) == 0));
  SET_CARRY_FLAG((bool)bit7);
}

// Shift right logical (b7=0)
void bit_operation_SRL(u8* reg)
{
  u8 bit0 = *reg & 0x01;
  *reg >>= 1;

  SET_HALF_FLAG(0);
  SET_SUB_FLAG(0);
  SET_ZERO_FLAG((*reg == 0));
  SET_CARRY_FLAG((bool)bit0);
}

void bit_operation_SRL_u16(u16 reg)
{
  u8 bit0 = read_uint8_data(reg) & 0x01;
  u8 value = read_uint8_data(reg);
  write_uint8_data(reg, value >>= 1);

  SET_HALF_FLAG(0);
  SET_SUB_FLAG(0);
  SET_ZERO_FLAG((read_uint8_data(reg) == 0));
  SET_CARRY_FLAG((bool)bit0);
}

// Shift right arithmetic (b7=b7)
void bit_operation_SRA(u8* reg)
{
  u8 bit0 = *reg & 0x01;
  u8 bit7 = *reg & 0x80;
  *reg = (*reg >> 1) + bit7;

  SET_HALF_FLAG(0);
  SET_SUB_FLAG(0);
  SET_ZERO_FLAG((*reg == 0));
  SET_CARRY_FLAG((bool)bit0);
}

void bit_operation_SRA_u16(u16 reg)
{
  u8 bit0 = read_uint8_data(reg) & 0x01;
  u8 bit7 = read_uint8_data(reg) & 0x80;
  write_uint8_data(reg, (read_uint8_data(reg) >> 1) + bit7);

  SET_HALF_FLAG(0);
  SET_SUB_FLAG(0);
  SET_ZERO_FLAG((read_uint8_data(reg) == 0));
  SET_CARRY_FLAG((bool)bit0);
}

// Sets a bit to 1
void bit_operation_set(u8* reg, int bit)
{
  u8 bit_mask = 1 << bit;
  u8 value = *reg | bit_mask;
  *reg = value;
}
void bit_operation_set_u16(u16 reg, int bit)
{
  u8 bit_mask = 1 << bit;
  u8 value = read_uint8_data(reg) | bit_mask;
  write_uint8_data(reg, value);
}

void cb_prefix_instruction(instruction inst)
{
  u8 prefix = read_uint8_data(cpu.reg.pc++);
  switch (prefix) {
    case 0x37:  // Swap upper and lower nibbles of register A, flags updated
      bit_operation_swap(&cpu.reg.a);
      cycle_cpu(8);
      break;
    case 0x30:  // Swap upper and lower nibbles of register B, flags updated
      bit_operation_swap(&cpu.reg.b);
      cycle_cpu(8);
      break;
    case 0x31:  // Swap upper and lower nibbles of register C, flags updated
      bit_operation_swap(&cpu.reg.c);
      cycle_cpu(8);
      break;
    case 0x32:  // Swap upper and lower nibbles of register D, flags updated
      bit_operation_swap(&cpu.reg.d);
      cycle_cpu(8);
      break;
    case 0x33:  // Swap upper and lower nibbles of register E, flags updated
      bit_operation_swap(&cpu.reg.e);
      cycle_cpu(8);
      break;
    case 0x34:  // Swap upper and lower nibbles of register H, flags updated
      bit_operation_swap(&cpu.reg.h);
      cycle_cpu(8);
      break;
    case 0x35:  // Swap upper and lower nibbles of register L, flags updated
      bit_operation_swap(&cpu.reg.l);
      cycle_cpu(8);
      break;
    case 0x36:  // Swap upper and lower nibbles of byte at (HL), flags updated
      bit_operation_swap_u16(cpu_read_reg(reg_hl));
      cycle_cpu(16);
      break;
    case 0x07:  // Rotate contents of A register left and store Bit 7 in CF, flags updated
      bit_operation_RL(&cpu.reg.a,
                       true);  // Carry = true (Rotate left with Carry RLC A)
      cycle_cpu(8);
      break;
    case 0x00:  // Rotate contents of B register left and store Bit 7 in CF, flags updated
      bit_operation_RL(&cpu.reg.b,
                       true);  // Carry = true (Rotate left with Carry RLC B)
      cycle_cpu(8);
      break;
    case 0x01:  // Rotate contents of C register left and store Bit 7 in CF, flags updated
      bit_operation_RL(&cpu.reg.c,
                       true);  // Carry = true (Rotate left with Carry RLC C)
      cycle_cpu(8);
      break;
    case 0x02:  // Rotate contents of D register left and store Bit 7 in CF, flags updated
      bit_operation_RL(&cpu.reg.d,
                       true);  // Carry = true (Rotate left with Carry RLC D)
      cycle_cpu(8);
      break;
    case 0x03:  // Rotate contents of E register left and store Bit 7 in CF, flags updated
      bit_operation_RL(&cpu.reg.e,
                       true);  // Carry = true (Rotate left with Carry RLC E)
      cycle_cpu(8);
      break;
    case 0x04:  // Rotate contents of H register left and store Bit 7 in CF, flags updated
      bit_operation_RL(&cpu.reg.h,
                       true);  // Carry = true (Rotate left with Carry RLC H)
      cycle_cpu(8);
      break;
    case 0x05:  // Rotate contents of L register left and store Bit 7 in CF, flags updated
      bit_operation_RL(&cpu.reg.l,
                       true);  // Carry = true (Rotate left with Carry RLC L)
      cycle_cpu(8);
      break;
    case 0x06:  // Rotate contents of byte at (HL) left and store Bit 7 in CF, flags updated
      bit_operation_RL_u16(
          cpu_read_reg(reg_hl),
          true);  // Carry = true (Rotate left with Carry RLC L)
      cycle_cpu(16);
      break;
    case 0x17:  // Rotate contents of A register left through Carry flag and store Bit 7 in CF, flags updated
      bit_operation_RL(&cpu.reg.a, false);  // Carry = false (Rotate left RL A)
      cycle_cpu(8);
      break;
    case 0x10:  // Rotate contents of B register left through Carry flag and store Bit 7 in CF, flags updated
      bit_operation_RL(&cpu.reg.b,
                       false);  // Carry = false (Rotate left with Carry RL B)
      cycle_cpu(8);
      break;
    case 0x11:  // Rotate contents of C register left through Carry flag and store Bit 7 in CF, flags updated
      bit_operation_RL(&cpu.reg.c,
                       false);  // Carry = false (Rotate left with Carry RL C)
      cycle_cpu(8);
      break;
    case 0x12:  // Rotate contents of D register left through Carry flag and store Bit 7 in CF, flags updated
      bit_operation_RL(&cpu.reg.d,
                       false);  // Carry = false (Rotate left with Carry RL D)
      cycle_cpu(8);
      break;
    case 0x13:  // Rotate contents of E register left through Carry flag and store Bit 7 in CF, flags updated
      bit_operation_RL(&cpu.reg.e,
                       false);  // Carry = false (Rotate left with Carry RL E)
      cycle_cpu(8);
      break;
    case 0x14:  // Rotate contents of H register left through Carry flag and store Bit 7 in CF, flags updated
      bit_operation_RL(&cpu.reg.h,
                       false);  // Carry = false (Rotate left with Carry RL H)
      cycle_cpu(8);
      break;
    case 0x15:  // Rotate contents of L register left through Carry flag and store Bit 7 in CF, flags updated
      bit_operation_RL(&cpu.reg.l,
                       false);  // Carry = false (Rotate left with Carry RL L)
      cycle_cpu(8);
      break;
    case 0x16:  // Rotate contents of byte at (HL) lef through Carry flag and store Bit 7 in CF, flags updated
      bit_operation_RL_u16(
          cpu_read_reg(reg_hl),
          false);  // Carry = false (Rotate left with Carry RL L)
      cycle_cpu(16);
      break;
    case 0x0f:  // Rotate contents of A register right and store Bit 0 in CF, flags updated
      bit_operation_RR(&cpu.reg.a,
                       true);  // Carry = true (Rotate right with Carry RRC A)
      cycle_cpu(8);
      break;
    case 0x08:  // Rotate contents of B register right and store Bit 0 in CF, flags updated
      bit_operation_RR(&cpu.reg.b,
                       true);  // Carry = true (Rotate right with Carry RRC B)
      cycle_cpu(8);
      break;
    case 0x09:  // Rotate contents of C register right and store Bit 0 in CF, flags updated
      bit_operation_RR(&cpu.reg.c,
                       true);  // Carry = true (Rotate right with Carry RRC C)
      cycle_cpu(8);
      break;
    case 0x0a:  // Rotate contents of D register right and store Bit 0 in CF, flags updated
      bit_operation_RR(&cpu.reg.d,
                       true);  // Carry = true (Rotate right with Carry RRC D)
      cycle_cpu(8);
      break;
    case 0x0b:  // Rotate contents of E register right and store Bit 0 in CF, flags updated
      bit_operation_RR(&cpu.reg.e,
                       true);  // Carry = true (Rotate right with Carry RRC E)
      cycle_cpu(8);
      break;
    case 0x0c:  // Rotate contents of H register right and store Bit 0 in CF, flags updated
      bit_operation_RR(&cpu.reg.h,
                       true);  // Carry = true (Rotate right with Carry RRC H)
      cycle_cpu(8);
      break;
    case 0x0d:  // Rotate contents of L register right and store Bit 0 in CF, flags updated
      bit_operation_RR(&cpu.reg.l,
                       true);  // Carry = true (Rotate right with Carry RRC L)
      cycle_cpu(8);
      break;
    case 0x0e:  // Rotate contents of byte at (HL) right and store Bit 0 in CF, flags updated
      bit_operation_RR_u16(
          cpu_read_reg(reg_hl),
          true);  // Carry = true (Rotate right with Carry RRC (HL))
      cycle_cpu(16);
      break;
    case 0x1f:  // Rotate contents of A register right and store Bit 0 in CF, flags updated
      bit_operation_RR(&cpu.reg.a, false);  // Carry = false (Rotate right RR A)
      cycle_cpu(8);
      break;
    case 0x18:  // Rotate contents of B register right and store Bit 0 in CF, flags updated
      bit_operation_RR(&cpu.reg.b,
                       false);  // Carry = false (Rotate right with Carry RR B)
      cycle_cpu(8);
      break;
    case 0x19:  // Rotate contents of C register right and store Bit 0 in CF, flags updated
      bit_operation_RR(&cpu.reg.c,
                       false);  // Carry = false (Rotate right with Carry RR C)
      cycle_cpu(8);
      break;
    case 0x1a:  // Rotate contents of D register right and store Bit 0 in CF, flags updated
      bit_operation_RR(&cpu.reg.d,
                       false);  // Carry = false (Rotate right with Carry RR D)
      cycle_cpu(8);
      break;
    case 0x1b:  // Rotate contents of E register right and store Bit 0 in CF, flags updated
      bit_operation_RR(&cpu.reg.e,
                       false);  // Carry = false (Rotate right with Carry RR E)
      cycle_cpu(8);
      break;
    case 0x1c:  // Rotate contents of H register right and store Bit 0 in CF, flags updated
      bit_operation_RR(&cpu.reg.h,
                       false);  // Carry = false (Rotate right with Carry RR H)
      cycle_cpu(8);
      break;
    case 0x1d:  // Rotate contents of L register right and store Bit 0 in CF, flags updated
      bit_operation_RR(&cpu.reg.l,
                       false);  // Carry = false (Rotate right with Carry RR L)
      cycle_cpu(8);
      break;
    case 0x1e:  // Rotate contents of byte at (HL) right and store Bit 0 in CF, flags updated
      bit_operation_RR_u16(
          cpu_read_reg(reg_hl),
          false);  // Carry = false (Rotate right with Carry RR (HL))
      cycle_cpu(16);
      break;
    case 0x27:  // Shift contents of register A left and store Bit 7 in CF, Bit0 = 0, flags updated
      bit_operation_SLA(&cpu.reg.a);
      cycle_cpu(8);
      break;
    case 0x20:  // Shift contents of register B left and store Bit 7 in CF, Bit0 = 0, flags updated
      bit_operation_SLA(&cpu.reg.b);
      cycle_cpu(8);
      break;
    case 0x21:  // Shift contents of register C left and store Bit 7 in CF, Bit0 = 0, flags updated
      bit_operation_SLA(&cpu.reg.c);
      cycle_cpu(8);
      break;
    case 0x22:  // Shift contents of register D left and store Bit 7 in CF, Bit0 = 0, flags updated
      bit_operation_SLA(&cpu.reg.d);
      cycle_cpu(8);
      break;
    case 0x23:  // Shift contents of register E left and store Bit 7 in CF, Bit0 = 0, flags updated
      bit_operation_SLA(&cpu.reg.e);
      cycle_cpu(8);
      break;
    case 0x24:  // Shift contents of register H left and store Bit 7 in CF, Bit0 = 0, flags updated
      bit_operation_SLA(&cpu.reg.h);
      cycle_cpu(8);
      break;
    case 0x25:  // Shift contents of register L left and store Bit 7 in CF, Bit0 = 0, flags updated
      bit_operation_SLA(&cpu.reg.l);
      cycle_cpu(8);
      break;
    case 0x26:  // Shift contents of byte at (HL) left and store Bit 7 in CF, Bit0 = 0, flags updated
      bit_operation_SLA_u16(cpu_read_reg(reg_hl));
      cycle_cpu(16);
      break;
    case 0x2f:  // Shift contents of register A right and store Bit 0 in CF, Bit7 unchanged, flags updated
      bit_operation_SRA(&cpu.reg.a);
      cycle_cpu(8);
      break;
    case 0x28:  // Shift contents of register B right and store Bit 0 in CF, Bit7 unchanged, flags updated
      bit_operation_SRA(&cpu.reg.b);
      cycle_cpu(8);
      break;
    case 0x29:  // Shift contents of register C right and store Bit 0 in CF, Bit7 unchanged, flags updated
      bit_operation_SRA(&cpu.reg.c);
      cycle_cpu(8);
      break;
    case 0x2a:  // Shift contents of register D right and store Bit 0 in CF, Bit7 unchanged, flags updated
      bit_operation_SRA(&cpu.reg.d);
      cycle_cpu(8);
      break;
    case 0x2b:  // Shift contents of register E right and store Bit 0 in CF, Bit7 unchanged, flags updated
      bit_operation_SRA(&cpu.reg.e);
      cycle_cpu(8);
      break;
    case 0x2c:  // Shift contents of register H right and store Bit 0 in CF, Bit7 unchanged, flags updated
      bit_operation_SRA(&cpu.reg.h);
      cycle_cpu(8);
      break;
    case 0x2d:  // Shift contents of register L right and store Bit 0 in CF, Bit7 unchanged, flags updated
      bit_operation_SRA(&cpu.reg.l);
      cycle_cpu(8);
      break;
    case 0x2e:  // Shift contents byte at (HL) right and store Bit 0 in CF, Bit7 unchanged, flags updated
      bit_operation_SRA_u16(cpu_read_reg(reg_hl));
      cycle_cpu(16);
      break;
    case 0x3f:  // Shift contents of register A right and store Bit 0 in CF, Bit7 = 0, flags updated
      bit_operation_SRL(&cpu.reg.a);
      cycle_cpu(8);
      break;
    case 0x38:  // Shift contents of register B right and store Bit 0 in CF, Bit7 = 0, flags updated
      bit_operation_SRL(&cpu.reg.b);
      cycle_cpu(8);
      break;
    case 0x39:  // Shift contents of register C right and store Bit 0 in CF, Bit7 = 0, flags updated
      bit_operation_SRL(&cpu.reg.c);
      cycle_cpu(8);
      break;
    case 0x3a:  // Shift contents of register D right and store Bit 0 in CF, Bit7 = 0, flags updated
      bit_operation_SRL(&cpu.reg.d);
      cycle_cpu(8);
      break;
    case 0x3b:  // Shift contents of register E right and store Bit 0 in CF, Bit7 = 0, flags updated
      bit_operation_SRL(&cpu.reg.e);
      cycle_cpu(8);
      break;
    case 0x3c:  // Shift contents of register H right and store Bit 0 in CF, Bit7 = 0, flags updated
      bit_operation_SRL(&cpu.reg.h);
      cycle_cpu(8);
      break;
    case 0x3d:  // Shift contents of register L right and store Bit 0 in CF, Bit7 = 0, flags updated
      bit_operation_SRL(&cpu.reg.l);
      cycle_cpu(8);
      break;
    case 0x3e:  // Shift contents byte at (HL) right and store Bit 0 in CF, Bit7 = 0, flags updated
      bit_operation_SRL_u16(cpu_read_reg(reg_hl));
      cycle_cpu(16);
      break;
    case 0x47:  // Test Bit 0 in register A and set flags accordingly
      bit_operation_test(&cpu.reg.a, 0);
      cycle_cpu(8);
      break;
    case 0x40:  // Test Bit 0 in register B and set flags accordingly
      bit_operation_test(&cpu.reg.b, 0);
      cycle_cpu(8);
      break;
    case 0x41:  // Test Bit 0 in register C and set flags accordingly
      bit_operation_test(&cpu.reg.c, 0);
      cycle_cpu(8);
      break;
    case 0x42:  // Test Bit 0 in register D and set flags accordingly
      bit_operation_test(&cpu.reg.d, 0);
      cycle_cpu(8);
      break;
    case 0x43:  // Test Bit 0 in register E and set flags accordingly
      bit_operation_test(&cpu.reg.e, 0);
      cycle_cpu(8);
      break;
    case 0x44:  // Test Bit 0 in register H and set flags accordingly
      bit_operation_test(&cpu.reg.h, 0);
      cycle_cpu(8);
      break;
    case 0x45:  // Test Bit 0 in register L and set flags accordingly
      bit_operation_test(&cpu.reg.l, 0);
      cycle_cpu(8);
      break;
    case 0x46:  // Test Bit 0 in byte at (HL) and set flags accordingly
      bit_operation_test_u16(cpu_read_reg(reg_hl), 0);
      cycle_cpu(16);
      break;
    case 0x4f:  // Test Bit 1 in register A and set flags accordingly
      bit_operation_test(&cpu.reg.a, 1);
      cycle_cpu(8);
      break;
    case 0x48:  // Test Bit 1 in register B and set flags accordingly
      bit_operation_test(&cpu.reg.b, 1);
      cycle_cpu(8);
      break;
    case 0x49:  // Test Bit 1 in register C and set flags accordingly
      bit_operation_test(&cpu.reg.c, 1);
      cycle_cpu(8);
      break;
    case 0x4a:  // Test Bit 1 in register D and set flags accordingly
      bit_operation_test(&cpu.reg.d, 1);
      cycle_cpu(8);
      break;
    case 0x4b:  // Test Bit 1 in register E and set flags accordingly
      bit_operation_test(&cpu.reg.e, 1);
      cycle_cpu(8);
      break;
    case 0x4c:  // Test Bit 1 in register H and set flags accordingly
      bit_operation_test(&cpu.reg.h, 1);
      cycle_cpu(8);
      break;
    case 0x4d:  // Test Bit 1 in register L and set flags accordingly
      bit_operation_test(&cpu.reg.l, 1);
      cycle_cpu(8);
      break;
    case 0x4e:  // Test Bit 1 in byte at (HL) and set flags accordingly
      bit_operation_test_u16(cpu_read_reg(reg_hl), 1);
      cycle_cpu(16);
      break;
    case 0x57:  // Test Bit 2 in register A and set flags accordingly
      bit_operation_test(&cpu.reg.a, 2);
      cycle_cpu(8);
      break;
    case 0x50:  // Test Bit 2 in register B and set flags accordingly
      bit_operation_test(&cpu.reg.b, 2);
      cycle_cpu(8);
      break;
    case 0x51:  // Test Bit 2 in register C and set flags accordingly
      bit_operation_test(&cpu.reg.c, 2);
      cycle_cpu(8);
      break;
    case 0x52:  // Test Bit 2 in register D and set flags accordingly
      bit_operation_test(&cpu.reg.d, 2);
      cycle_cpu(8);
      break;
    case 0x53:  // Test Bit 2 in register E and set flags accordingly
      bit_operation_test(&cpu.reg.e, 2);
      cycle_cpu(8);
      break;
    case 0x54:  // Test Bit 2 in register H and set flags accordingly
      bit_operation_test(&cpu.reg.h, 2);
      cycle_cpu(8);
      break;
    case 0x55:  // Test Bit 2 in register L and set flags accordingly
      bit_operation_test(&cpu.reg.l, 2);
      cycle_cpu(8);
      break;
    case 0x56:  // Test Bit 2 in byte at (HL) and set flags accordingly
      bit_operation_test_u16(cpu_read_reg(reg_hl), 2);
      cycle_cpu(16);
      break;
    case 0x5f:  // Test Bit 3 in register A and set flags accordingly
      bit_operation_test(&cpu.reg.a, 3);
      cycle_cpu(8);
      break;
    case 0x58:  // Test Bit 3 in register B and set flags accordingly
      bit_operation_test(&cpu.reg.b, 3);
      cycle_cpu(8);
      break;
    case 0x59:  // Test Bit 3 in register C and set flags accordingly
      bit_operation_test(&cpu.reg.c, 3);
      cycle_cpu(8);
      break;
    case 0x5a:  // Test Bit 3 in register D and set flags accordingly
      bit_operation_test(&cpu.reg.e, 3);
      cycle_cpu(8);
      break;
    case 0x5b:  // Test Bit 3 in register E and set flags accordingly
      bit_operation_test(&cpu.reg.d, 3);
      cycle_cpu(8);
      break;
    case 0x5c:  // Test Bit 3 in register H and set flags accordingly
      bit_operation_test(&cpu.reg.h, 3);
      cycle_cpu(8);
      break;
    case 0x5d:  // Test Bit 3 in register L and set flags accordingly
      bit_operation_test(&cpu.reg.l, 3);
      cycle_cpu(8);
      break;
    case 0x5e:  // Test Bit 3 in byte at (HL) and set flags accordingly
      bit_operation_test_u16(cpu_read_reg(reg_hl), 3);
      cycle_cpu(16);
      break;
    case 0x67:  // Test Bit 4 in register A and set flags accordingly
      bit_operation_test(&cpu.reg.a, 4);
      cycle_cpu(8);
      break;
    case 0x60:  // Test Bit 4 in register B and set flags accordingly
      bit_operation_test(&cpu.reg.b, 4);
      cycle_cpu(8);
      break;
    case 0x61:  // Test Bit 4 in register C and set flags accordingly
      bit_operation_test(&cpu.reg.c, 4);
      cycle_cpu(8);
      break;
    case 0x62:  // Test Bit 4 in register D and set flags accordingly
      bit_operation_test(&cpu.reg.d, 4);
      cycle_cpu(8);
      break;
    case 0x63:  // Test Bit 4 in register E and set flags accordingly
      bit_operation_test(&cpu.reg.e, 4);
      cycle_cpu(8);
      break;
    case 0x64:  // Test Bit 4 in register H and set flags accordingly
      bit_operation_test(&cpu.reg.h, 4);
      cycle_cpu(8);
      break;
    case 0x65:  // Test Bit 4 in register L and set flags accordingly
      bit_operation_test(&cpu.reg.l, 4);
      cycle_cpu(8);
      break;
    case 0x66:  // Test Bit 4 in byte at (HL) and set flags accordingly
      bit_operation_test_u16(cpu_read_reg(reg_hl), 4);
      cycle_cpu(16);
      break;
    case 0x6f:  // Test Bit 5 in register A and set flags accordingly
      bit_operation_test(&cpu.reg.a, 5);
      cycle_cpu(8);
      break;
    case 0x68:  // Test Bit 5 in register B and set flags accordingly
      bit_operation_test(&cpu.reg.b, 5);
      cycle_cpu(8);
      break;
    case 0x69:  // Test Bit 5 in register C and set flags accordingly
      bit_operation_test(&cpu.reg.c, 5);
      cycle_cpu(8);
      break;
    case 0x6a:  // Test Bit 5 in register D and set flags accordingly
      bit_operation_test(&cpu.reg.d, 5);
      cycle_cpu(8);
      break;
    case 0x6b:  // Test Bit 5 in register E and set flags accordingly
      bit_operation_test(&cpu.reg.e, 5);
      cycle_cpu(8);
      break;
    case 0x6c:  // Test Bit 5 in register H and set flags accordingly
      bit_operation_test(&cpu.reg.h, 5);
      cycle_cpu(8);
      break;
    case 0x6d:  // Test Bit 5 in register L and set flags accordingly
      bit_operation_test(&cpu.reg.l, 5);
      cycle_cpu(8);
      break;
    case 0x6e:  // Test Bit 5 in byte at (HL) and set flags accordingly
      bit_operation_test_u16(cpu_read_reg(reg_hl), 5);
      cycle_cpu(16);
      break;
    case 0x77:  // Test Bit 6 in register A and set flags accordingly
      bit_operation_test(&cpu.reg.a, 6);
      cycle_cpu(8);
      break;
    case 0x70:  // Test Bit 6 in register B and set flags accordingly
      bit_operation_test(&cpu.reg.b, 6);
      cycle_cpu(8);
      break;
    case 0x71:  // Test Bit 6 in register C and set flags accordingly
      bit_operation_test(&cpu.reg.c, 6);
      cycle_cpu(8);
      break;
    case 0x72:  // Test Bit 6 in register D and set flags accordingly
      bit_operation_test(&cpu.reg.d, 6);
      cycle_cpu(8);
      break;
    case 0x73:  // Test Bit 6 in register E and set flags accordingly
      bit_operation_test(&cpu.reg.e, 6);
      cycle_cpu(8);
      break;
    case 0x74:  // Test Bit 6 in register H and set flags accordingly
      bit_operation_test(&cpu.reg.h, 6);
      cycle_cpu(8);
      break;
    case 0x75:  // Test Bit 6 in register L and set flags accordingly
      bit_operation_test(&cpu.reg.l, 6);
      cycle_cpu(8);
      break;
    case 0x76:  // Test Bit 6 in byte at (HL) and set flags accordingly
      bit_operation_test_u16(cpu_read_reg(reg_hl), 6);
      cycle_cpu(16);
      break;
    case 0x7f:  // Test Bit 7 in register A and set flags accordingly
      bit_operation_test(&cpu.reg.a, 7);
      cycle_cpu(8);
      break;
    case 0x78:  // Test Bit 7 in register B and set flags accordingly
      bit_operation_test(&cpu.reg.b, 7);
      cycle_cpu(8);
      break;
    case 0x79:  // Test Bit 7 in register C and set flags accordingly
      bit_operation_test(&cpu.reg.c, 7);
      cycle_cpu(8);
      break;
    case 0x7a:  // Test Bit 7 in register D and set flags accordingly
      bit_operation_test(&cpu.reg.d, 7);
      cycle_cpu(8);
      break;
    case 0x7b:  // Test Bit 7 in register E and set flags accordingly
      bit_operation_test(&cpu.reg.e, 7);
      cycle_cpu(8);
      break;
    case 0x7c:  // Test Bit 7 in register H and set flags accordingly
      bit_operation_test(&cpu.reg.h, 7);
      cycle_cpu(8);
      break;
    case 0x7d:  // Test Bit 7 in register L and set flags accordingly
      bit_operation_test(&cpu.reg.l, 7);
      cycle_cpu(8);
      break;
    case 0x7e:  // Test Bit 7 in byte at (HL) and set flags accordingly
      bit_operation_test_u16(cpu_read_reg(reg_hl), 7);
      cycle_cpu(16);
      break;
    case 0xc7:  // Set Bit 0 in register A, flags not affected
      bit_operation_set(&cpu.reg.a, 0);
      cycle_cpu(8);
      break;
    case 0xc0:  // Set Bit 0 in register B, flags not affected
      bit_operation_set(&cpu.reg.b, 0);
      cycle_cpu(8);
      break;
    case 0xc1:  // Set Bit 0 in register C, flags not affected
      bit_operation_set(&cpu.reg.c, 0);
      cycle_cpu(8);
      break;
    case 0xc2:  // Set Bit 0 in register D, flags not affectedy
      bit_operation_set(&cpu.reg.d, 0);
      cycle_cpu(8);
      break;
    case 0xc3:  // Set Bit 0 in register E, flags not affected
      bit_operation_set(&cpu.reg.e, 0);
      cycle_cpu(8);
      break;
    case 0xc4:  // Set Bit 0 in register H, flags not affected
      bit_operation_set(&cpu.reg.h, 0);
      cycle_cpu(8);
      break;
    case 0xc5:  // Set Bit 0 in register L, flags not affected
      bit_operation_set(&cpu.reg.l, 0);
      cycle_cpu(8);
      break;
    case 0xc6:  // Set Bit 0 in byte at (HL), flags not affected
      bit_operation_set_u16(cpu_read_reg(reg_hl), 0);
      cycle_cpu(16);
      break;
    case 0xcf:  // Set Bit 1 in register A, flags not affected
      bit_operation_set(&cpu.reg.a, 1);
      cycle_cpu(8);
      break;
    case 0xc8:  // Set Bit 1 in register B, flags not affected
      bit_operation_set(&cpu.reg.b, 1);
      cycle_cpu(8);
      break;
    case 0xc9:  // Set Bit 1 in register C, flags not affected
      bit_operation_set(&cpu.reg.c, 1);
      cycle_cpu(8);
      break;
    case 0xca:  // Set Bit 1 in register D, flags not affectedy
      bit_operation_set(&cpu.reg.d, 1);
      cycle_cpu(8);
      break;
    case 0xcb:  // Set Bit 1 in register E, flags not affected
      bit_operation_set(&cpu.reg.e, 1);
      cycle_cpu(8);
      break;
    case 0xcc:  // Set Bit 1 in register H, flags not affected
      bit_operation_set(&cpu.reg.h, 1);
      cycle_cpu(8);
      break;
    case 0xcd:  // Set Bit 1 in register L, flags not affected
      bit_operation_set(&cpu.reg.l, 1);
      cycle_cpu(8);
      break;
    case 0xce:  // Set Bit 1 in byte at (HL), flags not affected
      bit_operation_set_u16(cpu_read_reg(reg_hl), 1);
      cycle_cpu(16);
      break;
    case 0xd7:  // Set Bit 2 in register A, flags not affected
      bit_operation_set(&cpu.reg.a, 2);
      cycle_cpu(8);
      break;
    case 0xd0:  // Set Bit 2 in register B, flags not affected
      bit_operation_set(&cpu.reg.b, 2);
      cycle_cpu(8);
      break;
    case 0xd1:  // Set Bit 2 in register C, flags not affected
      bit_operation_set(&cpu.reg.c, 2);
      cycle_cpu(8);
      break;
    case 0xd2:  // Set Bit 2 in register D, flags not affectedy
      bit_operation_set(&cpu.reg.d, 2);
      cycle_cpu(8);
      break;
    case 0xd3:  // Set Bit 2 in register E, flags not affected
      bit_operation_set(&cpu.reg.e, 2);
      cycle_cpu(8);
      break;
    case 0xd4:  // Set Bit 2 in register H, flags not affected
      bit_operation_set(&cpu.reg.h, 2);
      cycle_cpu(8);
      break;
    case 0xd5:  // Set Bit 2 in register L, flags not affected
      bit_operation_set(&cpu.reg.l, 2);
      cycle_cpu(8);
      break;
    case 0xd6:  // Set Bit 2 in byte at (HL), flags not affected
      bit_operation_set_u16(cpu_read_reg(reg_hl), 2);
      cycle_cpu(16);
      break;
    case 0xdf:  // Set Bit 3 in register A, flags not affected
      bit_operation_set(&cpu.reg.a, 3);
      cycle_cpu(8);
      break;
    case 0xd8:  // Set Bit 3 in register B, flags not affected
      bit_operation_set(&cpu.reg.b, 3);
      cycle_cpu(8);
      break;
    case 0xd9:  // Set Bit 3 in register C, flags not affected
      bit_operation_set(&cpu.reg.c, 3);
      cycle_cpu(8);
      break;
    case 0xda:  // Set Bit 3 in register D, flags not affectedy
      bit_operation_set(&cpu.reg.d, 3);
      cycle_cpu(8);
      break;
    case 0xdb:  // Set Bit 3 in register E, flags not affected
      bit_operation_set(&cpu.reg.e, 3);
      cycle_cpu(8);
      break;
    case 0xdc:  // Set Bit 3 in register H, flags not affected
      bit_operation_set(&cpu.reg.h, 3);
      cycle_cpu(8);
      break;
    case 0xdd:  // Set Bit 3 in register L, flags not affected
      bit_operation_set(&cpu.reg.l, 3);
      cycle_cpu(8);
      break;
    case 0xde:  // Set Bit 3 in byte at (HL), flags not affected
      bit_operation_set_u16(cpu_read_reg(reg_hl), 3);
      cycle_cpu(16);
      break;
    case 0xe7:  // Set Bit 4 in register A, flags not affected
      bit_operation_set(&cpu.reg.a, 4);
      cycle_cpu(8);
      break;
    case 0xe0:  // Set Bit 4 in register B, flags not affected
      bit_operation_set(&cpu.reg.b, 4);
      cycle_cpu(8);
      break;
    case 0xe1:  // Set Bit 4 in register C, flags not affected
      bit_operation_set(&cpu.reg.c, 4);
      cycle_cpu(8);
      break;
    case 0xe2:  // Set Bit 4 in register D, flags not affectedy
      bit_operation_set(&cpu.reg.d, 4);
      cycle_cpu(8);
      break;
    case 0xe3:  // Set Bit 4 in register E, flags not affected
      bit_operation_set(&cpu.reg.e, 4);
      cycle_cpu(8);
      break;
    case 0xe4:  // Set Bit 4 in register H, flags not affected
      bit_operation_set(&cpu.reg.h, 4);
      cycle_cpu(8);
      break;
    case 0xe5:  // Set Bit 4 in register L, flags not affected
      bit_operation_set(&cpu.reg.l, 4);
      cycle_cpu(8);
      break;
    case 0xe6:  // Set Bit 4 in byte at (HL), flags not affected
      bit_operation_set_u16(cpu_read_reg(reg_hl), 4);
      cycle_cpu(16);
      break;
    case 0xef:  // Set Bit 5 in register A, flags not affected
      bit_operation_set(&cpu.reg.a, 5);
      cycle_cpu(8);
      break;
    case 0xe8:  // Set Bit 5 in register B, flags not affected
      bit_operation_set(&cpu.reg.b, 5);
      cycle_cpu(8);
      break;
    case 0xe9:  // Set Bit 5 in register C, flags not affected
      bit_operation_set(&cpu.reg.c, 5);
      cycle_cpu(8);
      break;
    case 0xea:  // Set Bit 5 in register D, flags not affectedy
      bit_operation_set(&cpu.reg.d, 5);
      cycle_cpu(8);
      break;
    case 0xeb:  // Set Bit 5 in register E, flags not affected
      bit_operation_set(&cpu.reg.e, 5);
      cycle_cpu(8);
      break;
    case 0xec:  // Set Bit 5 in register H, flags not affected
      bit_operation_set(&cpu.reg.h, 5);
      cycle_cpu(8);
      break;
    case 0xed:  // Set Bit 5 in register L, flags not affected
      bit_operation_set(&cpu.reg.l, 5);
      cycle_cpu(8);
      break;
    case 0xee:  // Set Bit 5 in byte at (HL), flags not affected
      bit_operation_set_u16(cpu_read_reg(reg_hl), 5);
      cycle_cpu(16);
      break;
    case 0xf7:  // Set Bit 6 in register A, flags not affected
      bit_operation_set(&cpu.reg.a, 6);
      cycle_cpu(8);
      break;
    case 0xf0:  // Set Bit 6 in register B, flags not affected
      bit_operation_set(&cpu.reg.b, 6);
      cycle_cpu(8);
      break;
    case 0xf1:  // Set Bit 6 in register C, flags not affected
      bit_operation_set(&cpu.reg.c, 6);
      cycle_cpu(8);
      break;
    case 0xf2:  // Set Bit 6 in register D, flags not affectedy
      bit_operation_set(&cpu.reg.d, 6);
      cycle_cpu(8);
      break;
    case 0xf3:  // Set Bit 6 in register E, flags not affected
      bit_operation_set(&cpu.reg.e, 6);
      cycle_cpu(8);
      break;
    case 0xf4:  // Set Bit 6 in register H, flags not affected
      bit_operation_set(&cpu.reg.h, 6);
      cycle_cpu(8);
      break;
    case 0xf5:  // Set Bit 6 in register L, flags not affected
      bit_operation_set(&cpu.reg.l, 6);
      cycle_cpu(8);
      break;
    case 0xf6:  // Set Bit 6 in byte at (HL), flags not affected
      bit_operation_set_u16(cpu_read_reg(reg_hl), 6);
      cycle_cpu(16);
      break;
    case 0xff:  // Set Bit 7 in register A, flags not affected
      bit_operation_set(&cpu.reg.a, 7);
      cycle_cpu(8);
      break;
    case 0xf8:  // Set Bit 7 in register B, flags not affected
      bit_operation_set(&cpu.reg.b, 7);
      cycle_cpu(8);
      break;
    case 0xf9:  // Set Bit 7 in register C, flags not affected
      bit_operation_set(&cpu.reg.c, 7);
      cycle_cpu(8);
      break;
    case 0xfa:  // Set Bit 7 in register D, flags not affectedy
      bit_operation_set(&cpu.reg.d, 7);
      cycle_cpu(8);
      break;
    case 0xfb:  // Set Bit 7 in register E, flags not affected
      bit_operation_set(&cpu.reg.e, 7);
      cycle_cpu(8);
      break;
    case 0xfc:  // Set Bit 7 in register H, flags not affected
      bit_operation_set(&cpu.reg.h, 7);
      cycle_cpu(8);
      break;
    case 0xfd:  // Set Bit 7 in register L, flags not affected
      bit_operation_set(&cpu.reg.l, 7);
      cycle_cpu(8);
      break;
    case 0xfe:  // Set Bit 7 in byte at (HL), flags not affected
      bit_operation_set_u16(cpu_read_reg(reg_hl), 7);
      cycle_cpu(16);
      break;
    case 0x87:  // Reset Bit 0 in register A, flags not affected
      bit_operation_reset(&cpu.reg.a, 0);
      cycle_cpu(8);
      break;
    case 0x80:  // Reset Bit 0 in register B, flags not affected
      bit_operation_reset(&cpu.reg.b, 0);
      cycle_cpu(8);
      break;
    case 0x81:  // Reset Bit 0 in register C, flags not affected
      bit_operation_reset(&cpu.reg.c, 0);
      cycle_cpu(8);
      break;
    case 0x82:  // Reset Bit 0 in register D, flags not affectedy
      bit_operation_reset(&cpu.reg.d, 0);
      cycle_cpu(8);
      break;
    case 0x83:  // Reset Bit 0 in register E, flags not affected
      bit_operation_reset(&cpu.reg.e, 0);
      cycle_cpu(8);
      break;
    case 0x84:  // Reset Bit 0 in register H, flags not affected
      bit_operation_reset(&cpu.reg.h, 0);
      cycle_cpu(8);
      break;
    case 0x85:  // Reset Bit 0 in register L, flags not affected
      bit_operation_reset(&cpu.reg.l, 0);
      cycle_cpu(8);
      break;
    case 0x86:  // Reset Bit 0 in byte at (HL), flags not affected
      bit_operation_reset_u16(cpu_read_reg(reg_hl), 0);
      cycle_cpu(16);
      break;
    case 0x8f:  // Reset Bit 1 in register A, flags not affected
      bit_operation_reset(&cpu.reg.a, 1);
      cycle_cpu(8);
      break;
    case 0x88:  // Reset Bit 1 in register B, flags not affected
      bit_operation_reset(&cpu.reg.b, 1);
      cycle_cpu(8);
      break;
    case 0x89:  // Reset Bit 1 in register C, flags not affected
      bit_operation_reset(&cpu.reg.c, 1);
      cycle_cpu(8);
      break;
    case 0x8a:  // Reset Bit 1 in register D, flags not affectedy
      bit_operation_reset(&cpu.reg.d, 1);
      cycle_cpu(8);
      break;
    case 0x8b:  // Reset Bit 1 in register E, flags not affected
      bit_operation_reset(&cpu.reg.e, 1);
      cycle_cpu(8);
      break;
    case 0x8c:  // Reset Bit 1 in register H, flags not affected
      bit_operation_reset(&cpu.reg.h, 1);
      cycle_cpu(8);
      break;
    case 0x8d:  // Reset Bit 1 in register L, flags not affected
      bit_operation_reset(&cpu.reg.l, 1);
      cycle_cpu(8);
      break;
    case 0x8e:  // Reset Bit 1 in byte at (HL), flags not affected
      bit_operation_reset_u16(cpu_read_reg(reg_hl), 1);
      cycle_cpu(16);
      break;
    case 0x97:  // Reset Bit 2 in register A, flags not affected
      bit_operation_reset(&cpu.reg.a, 2);
      cycle_cpu(8);
      break;
    case 0x90:  // Reset Bit 2 in register B, flags not affected
      bit_operation_reset(&cpu.reg.b, 2);
      cycle_cpu(8);
      break;
    case 0x91:  // Reset Bit 2 in register C, flags not affected
      bit_operation_reset(&cpu.reg.c, 2);
      cycle_cpu(8);
      break;
    case 0x92:  // Reset Bit 2 in register D, flags not affectedy
      bit_operation_reset(&cpu.reg.d, 2);
      cycle_cpu(8);
      break;
    case 0x93:  // Reset Bit 2 in register E, flags not affected
      bit_operation_reset(&cpu.reg.e, 2);
      cycle_cpu(8);
      break;
    case 0x94:  // Reset Bit 2 in register H, flags not affected
      bit_operation_reset(&cpu.reg.h, 2);
      cycle_cpu(8);
      break;
    case 0x95:  // Reset Bit 2 in register L, flags not affected
      bit_operation_reset(&cpu.reg.l, 2);
      cycle_cpu(8);
      break;
    case 0x96:  // Reset Bit 2 in byte at (HL), flags not affected
      bit_operation_reset_u16(cpu_read_reg(reg_hl), 2);
      cycle_cpu(16);
      break;
    case 0x9f:  // Reset Bit 3 in register A, flags not affected
      bit_operation_reset(&cpu.reg.a, 3);
      cycle_cpu(8);
      break;
    case 0x98:  // Reset Bit 3 in register B, flags not affected
      bit_operation_reset(&cpu.reg.b, 3);
      cycle_cpu(8);
      break;
    case 0x99:  // Reset Bit 3 in register C, flags not affected
      bit_operation_reset(&cpu.reg.c, 3);
      cycle_cpu(8);
      break;
    case 0x9a:  // Reset Bit 3 in register D, flags not affectedy
      bit_operation_reset(&cpu.reg.d, 3);
      cycle_cpu(8);
      break;
    case 0x9b:  // Reset Bit 3 in register E, flags not affected
      bit_operation_reset(&cpu.reg.e, 3);
      cycle_cpu(8);
      break;
    case 0x9c:  // Reset Bit 3 in register H, flags not affected
      bit_operation_reset(&cpu.reg.h, 3);
      cycle_cpu(8);
      break;
    case 0x9d:  // Reset Bit 3 in register L, flags not affected
      bit_operation_reset(&cpu.reg.l, 3);
      cycle_cpu(8);
      break;
    case 0x9e:  // Reset Bit 3 in byte at (HL), flags not affected
      bit_operation_reset_u16(cpu_read_reg(reg_hl), 3);
      cycle_cpu(16);
      break;
    case 0xa7:  // Reset Bit 4 in register A, flags not affected
      bit_operation_reset(&cpu.reg.a, 4);
      cycle_cpu(8);
      break;
    case 0xa0:  // Reset Bit 4 in register B, flags not affected
      bit_operation_reset(&cpu.reg.b, 4);
      cycle_cpu(8);
      break;
    case 0xa1:  // Reset Bit 4 in register C, flags not affected
      bit_operation_reset(&cpu.reg.c, 4);
      cycle_cpu(8);
      break;
    case 0xa2:  // Reset Bit 4 in register D, flags not affectedy
      bit_operation_reset(&cpu.reg.d, 4);
      cycle_cpu(8);
      break;
    case 0xa3:  // Reset Bit 4 in register E, flags not affected
      bit_operation_reset(&cpu.reg.e, 4);
      cycle_cpu(8);
      break;
    case 0xa4:  // Reset Bit 4 in register H, flags not affected
      bit_operation_reset(&cpu.reg.h, 4);
      cycle_cpu(8);
      break;
    case 0xa5:  // Reset Bit 4 in register L, flags not affected
      bit_operation_reset(&cpu.reg.l, 4);
      cycle_cpu(8);
      break;
    case 0xa6:  // Reset Bit 4 in byte at (HL), flags not affected
      bit_operation_reset_u16(cpu_read_reg(reg_hl), 4);
      cycle_cpu(16);
      break;
    case 0xaf:  // Reset Bit 5 in register A, flags not affected
      bit_operation_reset(&cpu.reg.a, 5);
      cycle_cpu(8);
      break;
    case 0xa8:  // Reset Bit 5 in register B, flags not affected
      bit_operation_reset(&cpu.reg.b, 5);
      cycle_cpu(8);
      break;
    case 0xa9:  // Reset Bit 5 in register C, flags not affected
      bit_operation_reset(&cpu.reg.c, 5);
      cycle_cpu(8);
      break;
    case 0xaa:  // Reset Bit 5 in register D, flags not affectedy
      bit_operation_reset(&cpu.reg.d, 5);
      cycle_cpu(8);
      break;
    case 0xab:  // Reset Bit 5 in register E, flags not affected
      bit_operation_reset(&cpu.reg.e, 5);
      cycle_cpu(8);
      break;
    case 0xac:  // Reset Bit 5 in register H, flags not affected
      bit_operation_reset(&cpu.reg.h, 5);
      cycle_cpu(8);
      break;
    case 0xad:  // Reset Bit 5 in register L, flags not affected
      bit_operation_reset(&cpu.reg.l, 5);
      cycle_cpu(8);
      break;
    case 0xae:  // Reset Bit 5 in byte at (HL), flags not affected
      bit_operation_reset_u16(cpu_read_reg(reg_hl), 5);
      cycle_cpu(16);
      break;
    case 0xb7:  // Reset Bit 6 in register A, flags not affected
      bit_operation_reset(&cpu.reg.a, 6);
      cycle_cpu(8);
      break;
    case 0xb0:  // Reset Bit 6 in register B, flags not affected
      bit_operation_reset(&cpu.reg.b, 6);
      cycle_cpu(8);
      break;
    case 0xb1:  // Reset Bit 6 in register C, flags not affected
      bit_operation_reset(&cpu.reg.c, 6);
      cycle_cpu(8);
      break;
    case 0xb2:  // Reset Bit 6 in register D, flags not affectedy
      bit_operation_reset(&cpu.reg.d, 6);
      cycle_cpu(8);
      break;
    case 0xb3:  // Reset Bit 6 in register E, flags not affected
      bit_operation_reset(&cpu.reg.e, 6);
      cycle_cpu(8);
      break;
    case 0xb4:  // Reset Bit 6 in register H, flags not affected
      bit_operation_reset(&cpu.reg.h, 6);
      cycle_cpu(8);
      break;
    case 0xb5:  // Reset Bit 6 in register L, flags not affected
      bit_operation_reset(&cpu.reg.l, 6);
      cycle_cpu(8);
      break;
    case 0xb6:  // Reset Bit 6 in byte at (HL), flags not affected
      bit_operation_reset_u16(cpu_read_reg(reg_hl), 6);
      cycle_cpu(16);
      break;
    case 0xbf:  // Reset Bit 7 in register A, flags not affected
      bit_operation_reset(&cpu.reg.a, 7);
      cycle_cpu(8);
      break;
    case 0xb8:  // Reset Bit 7 in register B, flags not affected
      bit_operation_reset(&cpu.reg.b, 7);
      cycle_cpu(8);
      break;
    case 0xb9:  // Reset Bit 7 in register C, flags not affected
      bit_operation_reset(&cpu.reg.c, 7);
      cycle_cpu(8);
      break;
    case 0xba:  // Reset Bit 7 in register D, flags not affectedy
      bit_operation_reset(&cpu.reg.d, 7);
      cycle_cpu(8);
      break;
    case 0xbb:  // Reset Bit 7 in register E, flags not affected
      bit_operation_reset(&cpu.reg.e, 7);
      cycle_cpu(8);
      break;
    case 0xbc:  // Reset Bit 7 in register H, flags not affected
      bit_operation_reset(&cpu.reg.h, 7);
      cycle_cpu(8);
      break;
    case 0xbd:  // Reset Bit 7 in register L, flags not affected
      bit_operation_reset(&cpu.reg.l, 7);
      cycle_cpu(8);
      break;
    case 0xbe:  // Reset Bit 7 in byte at (HL), flags not affected
      bit_operation_reset_u16(cpu_read_reg(reg_hl), 7);
      cycle_cpu(16);
      break;
    default:
      _ERROR(
          "in_cb_prefix_proc_error_invalid_immediate_u8, cannot load "
          "instruction PC: " PRINTF_ERROR_PC_REG)
      _CRITICAL
  }
}
