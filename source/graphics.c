#include "common.h"
#include <graphics.h>

pixel_object graphics_buffer[160 * 144];

graphics_tile convert_memory_to_tile(u8 bytes[16]) {
    
    graphics_tile tile;
    // per line 
    for(int x = 0; x < 16; NULL) {
        u8 lsb = bytes[x];
        u8 msb = bytes[x + 1];
        for(uint y = 7; y >= 0 ; y--) { 
            u8 value = pow(2 , y);
            pixel_object pixel;
            pixel.pixel_color = ((msb & value) << 1) & (lsb & value);
            if(x == 0) {
                tile.pixels_colors[x] = pixel;
            } else {
                tile.pixels_colors[x / 2] = pixel;
            }
        }
        x + 2;
    }
    

    return tile;
}
