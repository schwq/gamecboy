#include <dma.h>
#include <io.h>
#include <lcd.h>

lcd_context lcd = {0};

static const u32 dmg_palettes[4] = {
    0xFFFFFFFF,  // White
    0xFFC0C0C0,  // Light Gray
    0xFF606060,  // Dark Gray
    0xFF000000   // Black
};

void lcd_init()
{
  lcd.lcdc = 0x91;
  lcd.stat = 0x85;
  lcd.scy = 0;
  lcd.scx = 0;
  lcd.ly = 0;
  lcd.lyc = 0;
  lcd.dma = 0xFF;
  lcd.bg_palette = 0xFC;
  lcd.obj0_palette = 0xFF;
  lcd.obj1_palette = 0xFF;
  lcd.wx = 0;
  lcd.wy = 0;

  FOR_I(i, 4)
  {
    lcd.bg_palette_colors[i] = dmg_palettes[i];
    lcd.obj0_palette_colors[i] = dmg_palettes[i];
    lcd.obj1_palette_colors[i] = dmg_palettes[i];
  }
}

void update_palette(u8 offset, u8 value)
{
  u32* colors = lcd.bg_palette_colors;
  switch (offset) {
    case 1:
      colors = lcd.obj0_palette_colors;
      break;
    case 2:
      colors = lcd.obj1_palette_colors;
      break;
  }
  colors[0] = dmg_palettes[(value & 0x03)];
  colors[1] = dmg_palettes[((value >> 2) & 0x03)];
  colors[2] = dmg_palettes[((value >> 4) & 0x03)];
  colors[3] = dmg_palettes[((value >> 6) & 0x03)];
}

void lcd_write(u16 addr, u8 value)
{
  addr -= IO_LCD_ADDR_STR;
  u8* p = (u8*)&lcd;
  p[addr] = value;

  switch (addr) {
    case 6:
      dma_start(value);
      break;
    case 7:
      update_palette(0, value);
      break;
    case 8:
      update_palette(1, value);
      break;
    case 9:
      update_palette(2, value);
      break;
  }
}

u8 lcd_read(u16 addr)
{
  addr -= IO_LCD_ADDR_STR;
  u8* p = (u8*)&lcd;
  return p[addr];
}