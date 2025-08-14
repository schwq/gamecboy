#include <ppu.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "graphics.h"

void init_ppu()
{
  const size_t sz = EMULATOR_SCREEN_X * EMULATOR_SCREEN_Y;
  framebuffer = (u8*)malloc(sz * sizeof(u8));
  if (!framebuffer) {
    _ERROR("Failed to allocate framebuffer");
    _CRITICAL
  }

  memset(framebuffer, 0x00, sz);
}

void terminate_ppu()
{
  free(framebuffer);
}
// drawing pixels
void execute_ppu_mode3() {}

// oam scan
void execute_ppu_mode2() {}
// hblank
void execute_ppu_mode0()
{
  current_ppu_mode = kHBLANK;
}

void execute_ppu_mode1()
{
  current_ppu_mode = kVBLANK;
}

// TODO review this
void draw_background_tiles()
{
  graphics_tile_map map =
      generate_map(convert_lcdc_tile_map_bit_to_addr(get_lcdc_bg_tile_map()));
  graphics_ctx.background.tiles_map = map;
  for (int x = 0; x < BACKGROUND_SCREEN_X; x++) {
    for (int y = 0; y < BACKGROUND_SCREEN_Y; y++) {
      pixel_object output_pixel;
      u8 tile_map_x = BACKGROUND_SCREEN_X / 8;
      u8 tile_map_y = BACKGROUND_SCREEN_Y / 8;
      u8 tile_pixel_pos_x = BACKGROUND_SCREEN_X % 8;
      u8 tile_pixel_pos_y = BACKGROUND_SCREEN_Y % 8;
      graphics_tile current_tile = map.tiles[tile_map_x][tile_map_y];
      output_pixel =
          current_tile.pixels_colors[tile_pixel_pos_x][tile_pixel_pos_y];
      background_buffer[x][y] = output_pixel;
    }
  }
}

void draw_ppu()  // represent a frame
{
  update_window_and_background_positions(&graphics_ctx.window,
                                         &graphics_ctx.background);
  draw_background_tiles();
  create_graphics_objects();

  size_t sizecpy = sizeof(pixel_object) * EMULATOR_SCREEN_X * EMULATOR_SCREEN_Y;
  memcpy(framebuffer, &graphics_buffer, sizecpy);
}

void fifo_pixel_fetcher()
{
  u16 tile_map_addr_used;

  if (get_lcdc_bg_tile_map() && !verify_scanline_inside_window()) {
    tile_map_addr_used = TILE_MAP_ADDR_1;
  }
  else if (get_lcdc_bg_window_tiles() && verify_scanline_inside_window()) {
    tile_map_addr_used = TILE_MAP_ADDR_1;
  }
  else {
    tile_map_addr_used = TILE_MAP_ADDR_0;
  }

  //fetched_tile pixels_extracted_from_tile = fifo_get_tile(tile_map_addr_used);
}

void reset_screen()
{
  for (int x = 0; x < BACKGROUND_SCREEN_X; x++) {
    for (int y = 0; y < BACKGROUND_SCREEN_Y; y++) {
      background_buffer[x][y].pixel_color = PIXEL_COLOR_WHITE;
    }
  }
}
