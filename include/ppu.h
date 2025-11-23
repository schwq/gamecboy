#pragma once

#include <common.h>

#define LCD_WIDTH 160
#define LCD_HEIGHT 144

typedef struct {

  u8 vram[0x2000];

} ppu_context;

extern ppu_context ppu;

void ppu_init();
void ppu_tick();

u8 ppu_read_vram(u16 addr);
void ppu_write_vram(u16 addr, u8 value);

u8 ppu_read_oam(u16 addr);
void ppu_write_oam(u16 addr, u8 value);