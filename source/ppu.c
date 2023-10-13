#include "graphics.h"
#include <ppu.h>
#include <sys/types.h>

void gain_video_memory_access(access_memory_source source) {
    ppu_ctx.video_access_control.source = source;
}
access_memory_source video_memory_source() {
    return ppu_ctx.video_access_control.source;
}

void draw_background_tiles() {
    
}

void draw_ppu() {
    
    

}

void fifo_pixel_fetcher() {
    u16 tile_map_addr_used;

    if(get_lcdc_bg_tile_map() && !verify_scanline_inside_window()) {
        tile_map_addr_used =  TILE_MAP_ADDR_1;
    } else if(get_lcdc_bg_window_tiles() && verify_scanline_inside_window()) {
        tile_map_addr_used = TILE_MAP_ADDR_1;
    } else {
        tile_map_addr_used = TILE_MAP_ADDR_0;
    }
    
    fetched_tile pixels_extracted_from_tile = fifo_get_tile(tile_map_addr_used); 
}

void reset_screen() {
    for(int x = 0; x < BACKGROUND_SCREEN_X; x++) {
        for(int y = 0; y < BACKGROUND_SCREEN_Y; y++) {
            background_buffer[x][y].pixel_color = PIXEL_COLOR_WHITE;
        }
    }
}
