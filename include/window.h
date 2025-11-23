#pragma once

#include <SDL2/SDL.h>
#include <common.h>
#include <ppu.h>

#define FRAMEBUFFER_SIZE LCD_HEIGHT* LCD_WIDTH

typedef struct {

  u32* framebuffer;
  SDL_Window* window;
  SDL_Renderer* renderer;
  SDL_Texture* texture;
  bool running;
  u32 frame;

} window_context;

extern window_context window;

int window_init(const char* title);
void window_run();
void window_terminate();