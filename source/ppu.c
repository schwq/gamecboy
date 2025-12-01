#include <assert.h>
#include <bus.h>
#include <cpu.h>
#include <ppu.h>
#include <window.h>

ppu_context ppu = {0};

void increment_ly()
{
  if (lcd.ly >= 154) {
    lcd.ly = 0;
  }
  else {
    lcd.ly++;
  }
  if (lcd.ly == lcd.lyc) {

    STAT_SET_LYC_LY(1);

    if (STAT_INT_CHECK(STAT_LYC)) {
      cpu_request_int(INT_LCD);
    }
  }
  else {
    STAT_SET_LYC_LY(0);
  }
}

void ppu_write_vram(u16 addr, u8 value)
{
  addr -= 0x8000;
  ppu.vram[addr] = value;
}

u8 ppu_read_vram(u16 addr)
{
  addr -= 0x8000;
  return ppu.vram[addr];
}

void ppu_write_oam(u16 addr, u8 value)
{
  if (addr >= 0xFE00) {
    addr -= 0xFE00;
  }
  u8* p = (u8*)&ppu.oam_ram;
  p[addr] = value;
}

u8 ppu_read_oam(u16 addr)
{
  if (addr >= 0xFE00) {
    addr -= 0xFE00;
  }
  u8* p = (u8*)&ppu.oam_ram;
  return p[addr];
}

void vblank_mode()
{
  if (ppu.stats.line_ticks >= TICKS_PER_LINE) {
    increment_ly();

    if (lcd.ly >= LINES_PER_FRAME) {
      PPU_SET_PPUMODE(PPUMODE_OAM_SCAN);
      if (STAT_INT_CHECK(STAT_OAM)) {
        cpu_request_int(INT_LCD);  // TODO: check this interrupt
      }

      lcd.ly = 0;
    }
    ppu.stats.line_ticks = 0;
  }
}

void fifo_reset()
{
  while (ppu.fifo.size > 0) {
    fifo_queue_pop();
  }
  ppu.fifo.head = NULL;
}

void hblank_mode()
{
  if (ppu.stats.line_ticks >= TICKS_PER_LINE) {
    increment_ly();

    if (lcd.ly >= LCD_HEIGHT) {
      PPU_SET_PPUMODE(PPUMODE_VBLANK);
      cpu_request_int(INT_VBLANK);

      if (STAT_INT_CHECK(STAT_VBLANK)) {
        cpu_request_int(INT_LCD);
      }

      if (cartridge.save) {
        cart_write_save();
      }
    }
    else {
      PPU_SET_PPUMODE(PPUMODE_OAM_SCAN);
    }
    ppu.stats.line_ticks = 0;
  }
}

void push_pixel_to_lcd()
{
  if (ppu.fifo.size >= 8) {
    u32 color = fifo_queue_pop();

    if (ppu.stats.line_x >= (lcd.scx % 8)) {
      window.framebuffer[ppu.stats.pushed_x + (lcd.ly * LCD_WIDTH)] = color;
      ppu.stats.pushed_x++;
    }
    ppu.stats.line_x++;
  }
}

bool is_window_on_screen()
{
  return LCDC_WND_ENABLE && lcd.wx >= 0 && lcd.wx <= LCD_WIDTH + 6 &&
         lcd.wy >= 0 && lcd.wy < LCD_HEIGHT;
}

void pl_load_window_tiles()
{
  if (!is_window_on_screen)
    return;

  if (ppu.stats.fetched_x + 7 >= lcd.wx &&
      ppu.stats.fetched_x + 7 < lcd.wx + LCD_HEIGHT + 14) {
    if (lcd.ly >= lcd.wy && lcd.ly < lcd.wy + LCD_WIDTH) {
      u8 tiley = ppu.stats.window_line / 8;

      ppu.stats.bgw_fetch_data[0] =
          bus_read(LCDC_WND_TILEMAP + ((ppu.stats.fetched_x + 7 - lcd.wx) / 8) +
                   (tiley * 32));

      if (LCDC_BG_WND_TILEDATA == 0x8800) {
        ppu.stats.bgw_fetch_data[0] += 128;
      }
    }
  }
}

void pl_load_sprite_tiles() {
  
}

void process_pixel_pl()
{
  ppu.stats.mapx = (ppu.stats.fetched_x + lcd.scx);
  ppu.stats.mapy = (lcd.ly + lcd.scy);
  ppu.stats.tile_y = (ppu.stats.mapy % 8) * 2;

  if (ppu.stats.line_ticks % 2 == 0) {
    switch (ppu.fifo_state.current_step) {
      case FPFS_GET_TILE: {

        if (LCDC_BG_WND_ENABLE) {
          ppu.stats.bgw_fetch_data[0] =
              bus_read(LCDC_BG_TILEMAP + (ppu.stats.mapx / 8) +
                       ((ppu.stats.mapy / 8) * 32));
          if (LCDC_BG_WND_TILEDATA == 0x8800) {
            ppu.stats.bgw_fetch_data[0] += 128;
          }

          pl_load_window_tiles();

          if (LCDC_OBJ_ENABLE && ppu.line_sprites) {
            pl_load_sprite_tiles();
          }
        }
        ppu.stats.fetched_x += 8;
        ppu.fifo_state.current_step = FPFS_GET_DATA_LOW;
      }

      break;
      case FPFS_GET_DATA_LOW: {
        ppu.fifo_state.current_step = FPFS_GET_DATA_HIGH;
      } break;
      case FPFS_GET_DATA_HIGH: {
        ppu.fifo_state.current_step = FPFS_SLEEP;
      } break;
      case FPFS_SLEEP: {
        ppu.fifo_state.current_step = FPFS_PUSH;
      } break;
      case FPFS_PUSH: {
        ppu.fifo_state.current_step = FPFS_GET_TILE;
      } break;
    }
  }

  push_pixel_to_lcd();
}

void drawing_mode()
{
  process_pixel_pl();

  if (ppu.stats.pushed_x >= LCD_WIDTH) {
    fifo_reset();

    PPU_SET_PPUMODE(PPUMODE_HBLANK);

    if (STAT_INT_CHECK(STAT_HBLANK)) {
      cpu_request_int(INT_LCD);
    }
  }
}

void sort_line_sprites(oam_attributes oam, oam_attributes_entry* entry)
{
  oam_attributes_entry* cur = ppu.line_sprites;  // Current line sprite
  oam_attributes_entry* prev = cur;              // Previous

  while (cur) {
    if (cur->value.x > oam.x) {  // Sorted by x position
      prev->next = entry;
      entry->next = cur;
      break;
    }

    if (!cur->next) {  // End of the list
      cur->next = entry;
      break;
    }

    prev = cur;  // Exchange values
    cur = cur->next;
  }
}

void load_line_sprites()
{
  u8 ly = lcd.ly;

  u8 obj_height = LCDC_OBJ_SIZE;

  FOR_I(n, 40)
  {
    oam_attributes oam = ppu.oam_ram[n];

    if (ppu.line_sprite_count >= 10)
      break;

    if (oam.x == 0)
      continue;  // Hidden

    if (oam.y <= ly + 16 && oam.y + obj_height >= ly + 16) {
      oam_attributes_entry* entry =
          &ppu.line_sprites_ram[ppu.line_sprite_count++];
      entry->value = oam;
      entry->next = NULL;

      if (!ppu.line_sprites || ppu.line_sprites->value.x > oam.x) {
        entry->next = ppu.line_sprites;
        ppu.line_sprites = entry;
        continue;
      }

      sort_line_sprites(oam, entry);

    }  // On the line
  }
}

void oam_scan_mode()
{
  if (ppu.stats.line_ticks == 1) {
    // Load oam sprites for this line only on the first tick
    ppu.line_sprite_count = 0;
    ppu.line_sprites = 0;

    load_line_sprites();
  }

  if (ppu.stats.line_ticks >= 80) {
    PPU_SET_PPUMODE(PPUMODE_DRAWING);
    ppu.stats.pushed_x = 0;
  }
}

void ppu_tick()
{
  ppu.stats.line_ticks++;
  switch (ppu.current_mode) {
    case PPUMODE_DRAWING:
      drawing_mode();
      break;
    case PPUMODE_HBLANK:
      hblank_mode();
      break;
    case PPUMODE_OAM_SCAN:
      oam_scan_mode();
      break;
    case PPUMODE_VBLANK:
      vblank_mode();
      break;
  }
}

void fifo_queue_push(u32 color)
{
  if (ppu.fifo.size >= 8) {
    logfmt(LWARN, "Maximun of 8 pixels reached in the FIFO!");
    return;
  }

  fifo_node* node = malloc(sizeof(fifo_node));
  node->color = color;
  node->next = NULL;

  if (ppu.fifo.size == 0) {
    ppu.fifo.head = ppu.fifo.tail = node;
    ppu.fifo.head->next = ppu.fifo.tail;
  }
  else {
    ppu.fifo.tail->next = node;
    ppu.fifo.tail = node;
  }
  ppu.fifo.size++;
}

u32 fifo_queue_pop()
{
  if (!ppu.fifo.head) {
    logfmt(LERROR, "No FIFO head found! Cannot pop value of it!");
    return 0xFF;
  }

  fifo_node* node = ppu.fifo.head;
  ppu.fifo.head = ppu.fifo.head->next;
  u32 color = node->color;
  FREE(node);
  ppu.fifo.size--;
  return color;
}
