#include <cpu.h>
#include <interrupt.h>

#define ELSEIF_C(cond) \
  else if (cond) {}

void int_push_pc(u16 addr)
{
  stack_push16(cpu.reg.pc);
  cpu.reg.pc = addr;
}

bool int_check(u16 addr, interrupt_type it)
{
  if (cpu.interrupt_flags & it && cpu.ie_register & it) {
    int_push_pc(addr);
    cpu.interrupt_flags &= ~it;
    cpu.control.halted = false;
    cpu.control.ime = false;
    return true;
  }
  return false;
}

void int_handle_interrupt()
{
  if (int_check(0x40, INT_VBLANK)) {}
  ELSEIF_C(int_check(0x48, INT_LCD))
  ELSEIF_C(int_check(0x50, INT_TIMER))
  ELSEIF_C(int_check(0x58, INT_SERIAL))
  ELSEIF_C(int_check(0x60, INT_JOYPAD))
}