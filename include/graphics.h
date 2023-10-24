#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_stdinc.h>
#include <common.h>
#include <ram.h>
#include <stddef.h>

#define EMULATOR_SCREEN_X 160
#define EMULATOR_SCREEN_Y 144
#define BACKGROUND_SCREEN_X 256
#define BACKGROUND_SCREEN_Y 256

// LCD status reg
#define LY_REGISTER_ADDR 0xFF44
#define LYC_REGISTER_ADDR 0xFF45
#define LCD_STATUS_ADDR 0xFF41
// both functions make compare and verification of the ly register and can raise a interrupt
bool compare_ly_lcd();
// ly values between 144 and 153 indicate vertical blank, meaning, outside the visible area
bool verify_ly_vblank();

// LCDC
#define LCDC_ADDR 0xFF40
// returns the 0xFF40 register
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
// BG & Window enable / priority [Different meaning in CGB Mode]: 0 = Off; 1 =
// On
int get_lcdc_bg_window_priority();

// Color and pixel, SDL2 colors and emulator converting functions are defined here
#define PIXEL_COLOR_WHITE 0b00
#define PIXEL_COLOR_BLACK 0b01
#define PIXEL_COLOR_MEDIUM 0b10
#define PIXEL_COLOR_LIGHT 0b11

#define BG_PALETTE_DATA_ADDR 0xFF47
#define OBP0_PALETTE_DATA_ADDR 0xFF48
#define OBP1_PALETTE_DATA_ADDR 0xFF49
// Holds the color value, self explain itself
typedef struct {
  short color_id;
} gb_color;

typedef struct {
  gb_color id_0;
  gb_color id_1;
  gb_color id_2;
  gb_color id_3;
} color_palette;

extern color_palette bg_color;
void assign_ids_to_bg_color();
color_palette dma_palette_bit_to_color_palette(short dma_palette_bit);

// Periods and Mode inside the graphics, used from controling and hardware and communicate with the cpu about the current state of the ppu
typedef enum {
  kVBLANK,         // Vertical blank
  kHBLANK,         // Horizontal blank
  kRENDER_WINDOW,  // rendering pixels in the render area visible to the user
  kNONE_PERIOD
} rendering_periods;

typedef enum { kMODE0, kMODE1, kMODE2, kMODE3, kMODE_NONE } ppu_mode;

extern ppu_mode current_ppu_mode;
extern rendering_periods current_period;

#define SCY_REGISTER_ADDR 0xFF42
#define SCX_REGISTER_ADDR 0xFF43

typedef struct {
  u8 x_coord;
  u8 y_coord;
} screen_coordinates;

extern screen_coordinates scanline_pos;
bool verify_scanline_inside_window();

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
extern pixel_object background_buffer[BACKGROUND_SCREEN_X][BACKGROUND_SCREEN_Y];
extern pixel_object graphics_buffer[EMULATOR_SCREEN_X][EMULATOR_SCREEN_Y];
extern Uint8* sdl2_pixels;

#define TILE_DATA_BLOCK0_START 0x8000
#define TILE_DATA_BLOCK0_END 0x87FF
#define TILE_DATA_BLOCK1_START 0x8800
#define TILE_DATA_BLOCK1_END 0x08FFF
#define TILE_DATA_BLOCK2_START 0x9000SDL_PIXELFORMAT_ARGB4444
#define TILE_DATA_BLOCK2_END 0x97FF

typedef enum {
  kADDR_MODE_8000 = 0,
  kADDR_MODE_8800 = 1
} vram_tile_data_addr_mode;

typedef struct {
  pixel_object pixels_colors[8][8];
  uint index;
  screen_coordinates coord;
} graphics_tile;

graphics_tile tile_from_index(u8 index);
// from_map_generator is set to true went this function is called by
// (generate_default_map), if so, the error printf would be printed 32 * 32
// times, we dont want that >:(
graphics_tile generate_default_tile(bool from_map_generator);
// tiles maps
#define TILE_MAP_ADDR_0 0x9800
#define TILE_MAP_ADDR_1 0x9C00
#define TILE_MAP_SIZE 32

u16 convert_lcdc_tile_map_bit_to_addr(short lcdc_value);

typedef enum {
  kTILE_MAP_BG_TYPE,
  kTILE_MAP_WINDOW_TYPE,
  kTILE_MAP_BOTH_TYPE,
  kTILE_MAP_ERROR_TYPE
} graphics_tile_map_type;

typedef struct {
  graphics_tile tiles[TILE_MAP_SIZE][TILE_MAP_SIZE];
  graphics_tile_map_type type;
} graphics_tile_map;

u16 get_window_tile_map_addr_from_lcdc_bit();
u16 get_bg_tile_map_addr_from_lcdc_bit();

graphics_tile_map generate_map(u16 tile_map_addr);
graphics_tile_map generate_default_map();
extern vram_tile_data_addr_mode tile_data_addr_mode;

void set_vram_tile_data_addr_mode(vram_tile_data_addr_mode mode);
graphics_tile convert_memory_bytes_to_pixels(u8 memory[16]);

typedef struct {
  graphics_tile_map tile_map;
  window_position position;
} gb_window_context;

typedef struct {
  graphics_tile_map tiles_map;
  background_viewport_coord viewport;
} gb_background_context;

void generate_window_and_background(gb_window_context* window_ctx,
                                    gb_background_context* background_ctx);
void update_window_and_background_positions(
    gb_window_context* window_ctx, gb_background_context* background_ctx);

typedef struct {
  u8 bytes[4];
  bool x_flip;
  bool y_flip;
  bool bg_priority;
  u16 oam_location;  // Drawing priority check
} object_attribute;

bool oam_flip_bit_to_bool(short bit);

typedef struct {
  graphics_tile tile_top;
  graphics_tile tile_bottom;
  bool is_8_16;
} object_tile_setup;

typedef struct {
  object_tile_setup tiles;
  object_attribute attributes;
  screen_coordinates coord;
  color_palette palette;
} graphics_object;

// tells the ppu how many scanlines this obj still overlap;
typedef struct {
  graphics_object* obj;
  size_t reimander_y;
  bool overlap;
} object_overlap_context;

object_overlap_context verify_obj_overlap_scanline(graphics_object* obj,
                                                   u8 scanline);
bool object_in_range_x(u8 line, graphics_object* obj);
bool object_in_range_y(u8 line, graphics_object* obj);
bool object_inside_object(graphics_object* obj1, graphics_object* obj2);

void object_selection_priority();
// return the object with Priority
graphics_object* object_drawing_priority(graphics_object* obj1,
                                         graphics_object* obj2);

// read the oam byte from ram again
void regenerate_object_attrib(graphics_object* obj, u16 addr);
// regenerate screen position for every selected object that the graphics ctx have selected for been displayed (10 objs)
void update_objects_coords();

typedef struct {
  graphics_object objects[10];
  size_t size_selected;
} selected_object_array;

typedef struct {
  graphics_tile tiles[384];
  graphics_object objects[40];
  selected_object_array selection_obj;
  gb_background_context background;
  gb_window_context window;
} graphics_context;

extern graphics_context graphics_ctx;

void create_graphics_objects();
void read_object_attribute_memory();

// objects attributes

screen_coordinates get_object_pos(graphics_object object);
object_tile_setup get_object_tile_index(graphics_object object);

//                  7	      6	      5	        4	          3 2	 1 0
//  Attributes	Priority	Y flip	X flip	DMG palette	    Bank
//  CGB palette Priority: 0 = No, 1 = BG and Window colors 1–3 are drawn over
//  this OBJ Y flip: 0 = Normal, 1 = Entire OBJ is vertically mirrored X flip: 0
//  = Normal, 1 = Entire OBJ is horizontally mirrored DMG palette [Non CGB Mode
//  only]: 0 = OBP0, 1 = OBP1 Bank [CGB Mode Only]: 0 = Fetch tile from VRAM
//  bank 0, 1 = Fetch tile from VRAM bank 1 CGB palette [CGB Mode Only]: Which
//  of OBP0–7 to use
u8 get_object_flags(graphics_object object);
// Priority: 0 = No, 1 = BG and Window colors 1–3 are drawn over this OBJ
short get_object_flag_priority(graphics_object object);
// Y flip: 0 = Normal, 1 = Entire OBJ is vertically mirrored
short get_object_flag_y_flip(graphics_object object);
// X flip: 0 = Normal, 1 = Entire OBJ is horizontally mirrored
short get_object_flag_x_flip(graphics_object object);
// DMG palette [Non CGB Mode only]: 0 = OBP0, 1 = OBP1
short get_object_flag_dmg_palette(graphics_object object);

void generate_graphics_context(graphics_context* graphics);

#endif
