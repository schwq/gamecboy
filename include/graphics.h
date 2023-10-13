#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <SDL2/SDL_pixels.h>
#include <common.h>
#include <ram.h>
#include <stddef.h>

#define EMULATOR_SCREEN_X 160 
#define EMULATOR_SCREEN_Y 140
#define BACKGROUND_SCREEN_X 256
#define BACKGROUND_SCREEN_Y 256 

// TODO declare where and write function
extern unsigned int scanline_pos;
bool verify_scanline_inside_window();

// LCD status reg 
#define LY_REGISTER_ADDR 0xFF44 
#define LYC_REGISTER_ADDR 0xFF45
#define LCD_STATUS_ADDR 0xFF41

bool compare_ly_lcd();
bool verify_ly_vblank();

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

#define PIXEL_COLOR_WHITE 0b00 
#define PIXEL_COLOR_BLACK 0b01
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

typedef enum {
    kVBLANK, // Vertical blank
    kHBLANK, // Horizontal blank 
    kRENDER_WINDOW, // rendering pixels in the render area visible to the user
    kNONE_PERIOD
} rendering_periods;

// object attribute memory (physics)
#define OAM_ADDR_START 0xFE00
#define OAM_ADDR_END 0xFE9F
#define OAM_ADDR_SIZE OAM_ADDR_END - OAM_ADDR_START
#define OAM_DMA_REG 0xFF46

extern rendering_periods current_period;

void write_to_oam();
void dma_transfer();

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
extern pixel_object background_buffer[BACKGROUND_SCREEN_X][BACKGROUND_SCREEN_Y];
extern pixel_object graphics_buffer[EMULATOR_SCREEN_X][EMULATOR_SCREEN_Y];

#define TILE_DATA_BLOCK0_START 0x8000
#define TILE_DATA_BLOCK0_END 0x87FF
#define TILE_DATA_BLOCK1_START 0x8800
#define TILE_DATA_BLOCK1_END 0x08FFF
#define TILE_DATA_BLOCK2_START 0x9000
#define TILE_DATA_BLOCK2_END 0x97FF

typedef enum {
    kADDR_MODE_8000 = 0,
    kADDR_MODE_8800 = 1
} vram_tile_data_addr_mode;

typedef struct {
    pixel_object pixels_colors[64];
    uint index;
    screen_coordinates coord; 
} graphics_tile;

graphics_tile tile_from_index(u8 index);
// from_map_generator is set to true went this function is called by (generate_default_map), if so, the error printf 
// would be printed 32 * 32 times, we dont want that >:(
graphics_tile generate_default_tile(bool from_map_generator);
// tiles maps 
#define TILE_MAP_ADDR_0 0x9800
#define TILE_MAP_ADDR_1 0x9C00 
#define TILE_MAP_SIZE 32 * 32 

typedef enum {
    kTILE_MAP_BG_TYPE,
    kTILE_MAP_WINDOW_TYPE
} graphics_tile_map_type;

typedef struct {
    graphics_tile tiles[TILE_MAP_SIZE];
    graphics_tile_map_type type;
} graphics_tile_map;

graphics_tile_map generate_map(u16 tile_map_addr);
graphics_tile_map generate_default_map();
extern vram_tile_data_addr_mode tile_data_addr_mode;

void set_vram_tile_data_addr_mode(vram_tile_data_addr_mode mode);
u16 get_tile_map_addr_from_lcdc_bit();
graphics_tile convert_memory_bytes_to_pixels(u8 memory[16]);

typedef struct {
    graphics_tile_map tile_map;
    window_position position;
} gb_window_context;

typedef struct {
    graphics_tile_map tiles_map;
    background_viewport_coord viewport;
} gb_background_context;

void generate_window_and_background(gb_window_context* window_ctx, gb_background_context* background_ctx);
void update_window_and_background_positions(gb_window_context* window_ctx, gb_background_context* background_ctx);

typedef struct {
    u8 bytes[4];
    u16 oam_location; // Drawing priority check
} object_attribute;

typedef struct {
    graphics_tile tile_top;
    graphics_tile tile_bottom;
    bool is_8_16;
} object_tile_setup;

typedef struct {
    object_tile_setup tiles;
    object_attribute attributes;
    screen_coordinates coord;
} graphics_object;

bool object_in_range_x(u8 line, graphics_object* obj);
bool object_in_range_y(u8 line, graphics_object* obj);
bool object_inside_object(graphics_object* obj1, graphics_object* obj2);

void object_selection_priority();
// return the object with Priority
graphics_object* object_drawing_priority(graphics_object* obj1, graphics_object* obj2);

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

//                  7	      6	      5	        4	          3	     2	 1 	 0
//  Attributes	Priority	Y flip	X flip	DMG palette	    Bank	CGB palette
//  Priority: 0 = No, 1 = BG and Window colors 1–3 are drawn over this OBJ
//  Y flip: 0 = Normal, 1 = Entire OBJ is vertically mirrored
//  X flip: 0 = Normal, 1 = Entire OBJ is horizontally mirrored
//  DMG palette [Non CGB Mode only]: 0 = OBP0, 1 = OBP1
//  Bank [CGB Mode Only]: 0 = Fetch tile from VRAM bank 0, 1 = Fetch tile from VRAM bank 1
//  CGB palette [CGB Mode Only]: Which of OBP0–7 to use
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
