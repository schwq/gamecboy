#pragma once

#include <common.h>

#define LCD_HEIGHT 144
#define LCD_WIDTH 160

typedef struct {

  u8 lcdc;
  u8 stat;

  u8 scy;
  u8 scx;

  u8 ly;
  u8 lyc;

  u8 dma;

  u8 bg_palette;
  u8 obj0_palette;
  u8 obj1_palette;

  u8 wy;
  u8 wx;

  u32 bg_palette_colors[4];
  u32 obj0_palette_colors[4];
  u32 obj1_palette_colors[4];

} lcd_context;

extern lcd_context lcd;

#define LCDC_BG_WND_ENABLE BIT(lcd.lcdc, 0)
#define LCDC_OBJ_ENABLE BIT(lcd.lcdc, 1)
#define LCDC_OBJ_SIZE (BIT(lcd.lcdc, 2) ? 16 : 8)
#define LCDC_BG_TILEMAP (BIT(lcd.lcdc, 3) ? 0x9C00 : 0x9800)
#define LCDC_BG_WND_TILEDATA (BIT(lcd.lcdc, 4) ? 0x8000 : 0x8800)
#define LCDC_WND_ENABLE BIT(lcd.lcdc, 5)
#define LCDC_WND_TILEMAP (BIT(lcd.lcdc, 6) ? 0x9C00 : 0x9800)
#define LCDC_LCD_PPU_ENABLE BIT(lcd.lcdc, 7)

typedef enum {

  PPUMODE_HBLANK = 0,
  PPUMODE_VBLANK,
  PPUMODE_OAM_SCAN,
  PPUMODE_DRAWING

} ppumode;

#define LCD_SET_PPUMODE(mode) (lcd.stat = ((lcd.stat & ~0b11) | mode))

typedef enum {

  STAT_LYC = (1 << 6),
  STAT_OAM = (1 << 5),
  STAT_VBLANK = (1 << 4),
  STAT_HBLANK = (1 << 3)

} stat_int_type;

#define STAT_SET_LYC_LY(on) BIT_SET(lcd.stat, 2, on)

#define STAT_INT_CHECK(type) lcd.stat& type

void lcd_init();
void lcd_write(u16 addr, u8 value);
u8 lcd_read(u16 addr);