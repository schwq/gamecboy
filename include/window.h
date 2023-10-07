#ifndef _WINDOW_H_
#define _WINDOW_H_
#include "common.h"
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event;
    u32 sdl_flags;
    bool shutdown;
} emulation_window;

extern emulation_window emu_window;

void init_window(u32 flags, const char* window_name, uint size_x, uint size_y);
void handle_window_input();
void render_window();
void terminate_window();

#endif
