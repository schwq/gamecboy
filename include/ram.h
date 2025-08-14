#pragma once

#include <common.h>

typedef struct {

  u8 wram[0x2000];  // 8kb
  u8 hram[0x80];    // 128 bits

} ram_context;

extern ram_context ram;

u8 wram_read(u16 addr);
u8 hram_read(u16 addr);

void wram_write(u16 addr, u8 value);
void hram_write(u16 addr, u8 value);
