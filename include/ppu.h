#pragma once

#include <common.h>
#include <lcd.h>

#define TICKS_PER_LINE 456
#define LINES_PER_FRAME 154
#define TICKS_OAM_SCAN 80

typedef struct _fifo_node {
  struct _fifo_node* next;
  u32 color;
} fifo_node;

typedef struct {
  fifo_node* head;
  fifo_node* tail;
  u32 size;
} fifo_queue;

void fifo_queue_push(u32 color);
u32 fifo_queue_pop();

void fifo_reset();

typedef struct {

  u8 line_ticks;

  u8 pushed_x;

  u8 mapx;
  u8 mapy;

  u8 tile_y;

  u8 fetched_x;

  u8 line_x;

  u8 pushed_x;

  u8 bgw_fetch_data[3];
  u8 oam_fetch_data[6];

  u8 window_line;

} ppu_stats;

typedef struct {

  u8 y;
  u8 x;
  u8 tile_index;
  u8 cgb_palette : 3;
  u8 bank : 1;
  u8 dmg_palette : 1;
  u8 xflip : 1;
  u8 yflip : 1;
  u8 priority : 1;

} oam_attributes;

typedef struct _oam_attributes_entry {
  oam_attributes value;
  struct _oam_attributes_entry* next;
} oam_attributes_entry;

typedef enum {

  FPFS_GET_TILE,
  FPFS_GET_DATA_LOW,
  FPFS_GET_DATA_HIGH,
  FPFS_SLEEP,
  FPFS_PUSH

} fifo_pixel_fetcher_step;

typedef struct {

  fifo_pixel_fetcher_step current_step;
  u8 tile_x;
  u8 tile_y;

} fifo_pixel_state;

typedef struct {
  ppumode current_mode;
  ppu_stats stats;
  fifo_queue fifo;
  fifo_pixel_state fifo_state;

  u8 vram[0x2000];  // 8192 bytes
  oam_attributes oam_ram[40];

  oam_attributes_entry* line_sprites;
  oam_attributes_entry line_sprites_ram[10];
  u8 line_sprite_count;

} ppu_context;

extern ppu_context ppu;

void ppu_write_vram(u16 addr, u8 value);
u8 ppu_read_vram(u16 addr);

void ppu_write_oam(u16 addr, u8 value);
u8 ppu_read_oam(u16 addr);

#define PPU_SET_PPUMODE(mode) \
  {                           \
    ppu.current_mode = mode;  \
    LCD_SET_PPUMODE(mode);    \
  }

void increment_ly();
void ppu_tick();
void vblank_mode();
void hblank_mode();
void drawing_mode();
void oam_scan_mode();