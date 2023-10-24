#ifndef _PPU_H_
#define _PPU_H_

#include <common.h>
#include <graphics.h>
#include "cpu.h"

#define MINIMUM_MODE3_LENGHT 172
extern int dots_count;

typedef struct {
  pixel_object pixels_hold[16];

} pixel_fifo;

typedef struct {
  pixel_object pixels[8];
  graphics_tile parent_tile;
} fetched_tile;

void init_ppu();
void reset_screen();
void draw_background_tiles();
void draw_ppu();
void ppu_cycle(int cycles);
void fifo_pixel_fetcher();
void terminate_ppu();
fetched_tile fifo_get_tile(u16 tile_map_addr);

typedef struct {
} ppu_context;

extern ppu_context ppu_ctx;

void execute_ppu_mode0();
void execute_ppu_mode1();
void execute_ppu_mode2();
void execute_ppu_mode3();

#endif
