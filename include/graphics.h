#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <SDL2/SDL_pixels.h>
#include <common.h>
#include <ram.h>

#define VRAM_START 0x8000
#define VRAM_END 0x9FFF
#define VRAM_SIZE VRAM_END - VRAM_START

// LCD status reg 
#define LY_REGISTER_ADDR 0xFF44 
#define LYC_REGISTER_ADDR 0xFF45
#define LCD_STATUS_ADDR 0xFF41

#define LCDC_ADDR 0xFF40
u8 get_lcdc();
// LCD & PPU enable: 0 = Off; 1 = On
int get_lcdc_lcd_ppu_enable();
// Window tile map area: 0 = 9800–9BFF; 1 = 9C00–9FFF
int get_lcdc_window_tile_map();
// Window enable: 0 = Off; 1 = On
int get_lcdc_window_enable();
// BG & Window tile data area: 0 = 8800–97FF; 1 = 8000–8FFF 
int get_lcdc_bg_window_tiles();
// BG tile map area: 0 = 9800–9BFF; 1 = 9C00–9FFF 
int get_lcdc_bg_tile_map();
// OBJ size: 0 = 8×8; 1 = 8×16 
int get_lcdc_obj_size();
// OBJ enable: 0 = Off; 1 = On 
int get_lcdc_obj_enable();
// BG & Window enable / priority [Different meaning in CGB Mode]: 0 = Off; 1 = On 
int get_lcdc_bg_window_priority();

#define PIXEL_COLOR_TRANS 0b00 
#define PIXEL_COLOR_DARK 0b01
#define PIXEL_COLOR_MEDIUM 0b10 
#define PIXEL_COLOR_LIGHT 0b11

#define BG_PALETTE_DATA_ADDR 0xFF47
#define OBP0_PALETTE_DATA_ADDR 0xFF48
#define OBP1_PALETTE_DATA_ADDR 0xFF49

typedef struct {
    short color_id; 
} gb_color;

typedef struct {
    gb_color id_0;
    gb_color id_1;
    gb_color id_2;
    gb_color id_3;
} color_palette;

void assign_bg_colors_to_ids(color_palette* palette);
void assign_obp_colors_to_ids(color_palette* obj_palette_0, color_palette* obj_palette_1);

// object attribute memory (physics)
#define OAM_START 0xFE00
#define OAM_END 0xFE9F
#define OAM_SIZE OAM_END - OAM_START

#define SCY_REGISTER_ADDR 0xFF42 
#define SCX_REGISTER_ADDR 0xFF43 

typedef struct {
    u8 x_coord;
    u8 y_coord;
} screen_coordinates;

typedef struct {
    screen_coordinates top_left;
    screen_coordinates bottom_right;
} background_viewport_coord;

screen_coordinates get_background_viewport_top_left();
screen_coordinates get_background_viewport_bottom_right();
background_viewport_coord get_background_viewport();

#define WY_REGISTER_ADDR 0xFF4A
#define WX_REGISTER_ADDR 0xFF4B

typedef struct {
    screen_coordinates top_left;
    screen_coordinates bottom_right;
} window_position;

screen_coordinates get_window_position_top_left_pos();
screen_coordinates get_window_position_bottom_right_pos();
window_position get_window_position();
bool verify_window_coordinates_range();

typedef struct {
    short pixel_color;
} pixel_object;

// video buffer, the current video being display
extern pixel_object graphics_buffer[160 * 144];

typedef struct {
    pixel_object pixels_colors[64];
    uint index;
    // some objects have one tile or two tiles, a bool is set to true when the obj is 2 tiles sized
    bool object_attach;
} graphics_tile;

// tiles maps 
#define TILE_MAP_ADDR_0 0x9800
#define TILE_MAP_ADDR_1 0x9C00
// TODO check if this isnt big enought to overload memory 
#define TILE_MAP_SIZE sizeof(graphics_tile) * (32 * 32) 

u32 get_tile_map_addr_from_lcdc_bit();


typedef struct {
    graphics_tile tile_1;
    graphics_tile tile_2;
    u32 object_attribute;
} graphics_object;

typedef struct {


} graphics_context;

graphics_tile convert_memory_to_tile(u8 bytes[16]);
graphics_object create_graphics_object(u32 attribute);

// TODO principal draw call, everything must be called from here 
void draw_graphics();

#endif
