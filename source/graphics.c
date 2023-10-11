#include "common.h"
#include "ram.h"
#include <graphics.h>

pixel_object graphics_buffer[160 * 144];

u32 get_tile_map_addr_from_lcdc_bit() {
    return (get_lcdc_window_tile_map() ? TILE_MAP_ADDR_1 : TILE_MAP_ADDR_0); 
}

u8 get_lcdc() {
    return read_uint8_data(LCDC_ADDR); 
}

// LCD & PPU enable: 0 = Off; 1 = On
int get_lcdc_lcd_ppu_enable() {
    return BIT(read_uint8_data(LCDC_ADDR), 7);
}
// Window tile map area: 0 = 9800–9BFF; 1 = 9C00–9FFF
int get_lcdc_window_tile_map() {
    return BIT(read_uint8_data(LCDC_ADDR), 6);
}
// Window enable: 0 = Off; 1 = On
int get_lcdc_window_enable() {
    return BIT(read_uint8_data(LCDC_ADDR), 5);
}
// BG & Window tile data area: 0 = 8800–97FF; 1 = 8000–8FFF 
int get_lcdc_bg_window_tiles() {
    return BIT(read_uint8_data(LCDC_ADDR), 4);
}
// BG tile map area: 0 = 9800–9BFF; 1 = 9C00–9FFF 
int get_lcdc_bg_tile_map() {
    return BIT(read_uint8_data(LCDC_ADDR), 3);
}
// OBJ size: 0 = 8×8; 1 = 8×16 
int get_lcdc_obj_size() {
    return BIT(read_uint8_data(LCDC_ADDR), 2);
}
// OBJ enable: 0 = Off; 1 = On 
int get_lcdc_obj_enable() {
    return BIT(read_uint8_data(LCDC_ADDR), 1);
}
// BG & Window enable / priority [Different meaning in CGB Mode]: 0 = Off; 1 = On 
int get_lcdc_bg_window_priority() {
    return BIT(read_uint8_data(LCDC_ADDR), 0);
}


screen_coordinates get_background_viewport_top_left() {
    screen_coordinates coord; 
    coord.x_coord = read_uint8_data(SCX_REGISTER_ADDR);
    coord.y_coord = read_uint8_data(SCY_REGISTER_ADDR);
    return coord;
}

screen_coordinates get_background_viewport_bottom_right() {
    screen_coordinates coord; 
    u8 x_pos = (read_uint8_data(SCX_REGISTER_ADDR) + 159) % 256;
    u8 y_pos = (read_uint8_data(SCY_REGISTER_ADDR) + 143) % 256;
    coord.x_coord = x_pos;
    coord.y_coord = y_pos;
    return coord;
}

background_viewport_coord get_background_viewport(){
    background_viewport_coord bg;
    bg.top_left  = get_background_viewport_top_left();
    bg.bottom_right = get_background_viewport_bottom_right();
    return bg;
}

screen_coordinates get_window_position_top_left_pos() {
    screen_coordinates coord; 
    coord.x_coord = read_uint8_data(WX_REGISTER_ADDR) + 7;
    coord.y_coord = read_uint8_data(WY_REGISTER_ADDR);
    return coord;
}

// TODO check if this is correct 
screen_coordinates get_window_position_bottom_right_pos() {
    screen_coordinates coord;
    coord.x_coord = read_uint8_data(WX_REGISTER_ADDR) + 7 + 166;
    coord.y_coord = read_uint8_data(WY_REGISTER_ADDR) + 7 + 143;
    return coord;
}

window_position get_window_position() {
    window_position window;
    window.top_left = get_window_position_top_left_pos();
    window.bottom_right = get_window_position_bottom_right_pos();
    
    if (!verify_window_coordinates_range()) {
        _ERROR("GB window position is out of range, verify window position have returned false! link to doc: https://gbdev.io/pandocs/Scrolling.html#ff4aff4b--wy-wx-window-y-position-x-position-plus-7");
    }

    return window;
}

bool verify_window_coordinates_range() {
    u8 wx = read_uint8_data(WX_REGISTER_ADDR);
    u8 wy = read_uint8_data(WY_REGISTER_ADDR);
    return ((wx >= 0 && wx <= 166) && (wy >= 0 && wy <= 143));
    
}


