#include "../include/ram.h"
#include "graphics.h"

ram_context ram = {0};

u8 wram_read(u16 addr)
{
  addr -= 0xC000;

  if (addr >= 0x2000) {
    logfmt(lerror, "WRAM memory read out of range " HEX_PATTERN, addr + 0xC000);
    _CRITICAL
    return 0xFF;
  }

  return ram.wram[addr];
}

u8 hram_read(u16 addr)
{
  addr -= 0xFF80;

  if (addr >= 0x80) {
    logfmt(lerror, "HRAM memory read out of range " HEX_PATTERN, addr + 0xFF80);
    _CRITICAL
    return 0xFF;
  }

  return ram.hram[addr];
}

void wram_write(u16 addr, u8 value)
{
  addr -= 0xC000;

  if (addr >= 0x2000) {
    logfmt(lerror, "WRAM memory write out of range " HEX_PATTERN,
           addr + 0xC000);
    _CRITICAL
    return;
  }

  ram.wram[addr] = value;
}

void hram_write(u16 addr, u8 value)
{
  addr -= 0xFF80;

  if (addr >= 0x80) {
    logfmt(lerror, "HRAM memory write out of range " HEX_PATTERN,
           addr + 0xFF80);
    _CRITICAL
    return;
  }

  ram.hram[addr] = value;
}
