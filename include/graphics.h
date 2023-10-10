#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <SDL2/SDL_pixels.h>
#include <common.h>
#include <ram.h>

#define VRAM_START 0x8000
#define VRAM_END 0x9FFF
#define VRAM_SIZE VRAM_END - VRAM_START

#define LCDC_ADDR 0xFF40

#define PIXEL_COLOR_TRANS 0x00 
#define PIXEL_COLOR_DARK 0x01
#define PIXEL_COLOR_MEDIUM 0x10 
#define PIXEL_COLOR_LIGHT 0x11 

// object attribute memory (physics)
#define OAM_START 0xFE00
#define OAM_END 0xFE9F
#define OAM_SIZE OAM_END - OAM_START

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

typedef struct {
    graphics_tile tile_1;
    graphics_tile tile_2;
    u32 object_attribute;
} graphics_object;

typedef struct {


} graphics_context;

graphics_tile convert_memory_to_tile(u8 bytes[16]);
graphics_object create_graphics_object(u32 attribute);

#endif
