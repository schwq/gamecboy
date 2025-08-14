#include "../include/window.h"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include "../include/common.h"
#include "graphics.h"
emulation_window emu_window;
// this must be called before malloc create pointers to cpu and ram! otherwise we are going to have a leak!
// TODO: implementation and management of malloc and pointer in case of _CRITICAL call and errors!
void init_window(u32 flags, const char* window_name, uint size_x, uint size_y)
{

  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    _ERROR("Cannot initialize SDL2!");
    _CRITICAL
  }

  emu_window.sdl_flags = flags;
  emu_window.shutdown = false;

  emu_window.window =
      SDL_CreateWindow(window_name, EMULATOR_SCREEN_X, EMULATOR_SCREEN_Y, 620,
                       387, SDL_WINDOW_SHOWN);
  if (emu_window.window == NULL) {
    _ERROR("Cannot initilize SDL2 window!");
    _CRITICAL
  }

  emu_window.renderer =
      SDL_CreateRenderer(emu_window.window, -1, emu_window.sdl_flags);
  if (emu_window.renderer == NULL) {
    _ERROR("Cannot initialize SDL2 Renderer!");
    SDL_DestroyWindow(emu_window.window);
    SDL_Quit();
    _CRITICAL
  }

  emu_window.texture = SDL_CreateTexture(
      emu_window.renderer, SDL_PIXELFORMAT_ARGB8888,
      SDL_TEXTUREACCESS_STREAMING, EMULATOR_SCREEN_X, EMULATOR_SCREEN_Y);

  printf("Created SDL2 window!\n");
}

void handle_window_input()
{
  SDL_Event event;
  while (SDL_PollEvent(&event)) {

    switch (event.type) {
      case SDL_QUIT:
        emu_window.shutdown = true;
        break;
      case SDL_KEYDOWN:
        gamepad_set_keysym(event.key.keysym.sym, true);
        break;
      case SDL_KEYUP:
        gamepad_set_keysym(event.key.keysym.sym, false);
        break;
    }

    switch (emu_window.event.key.keysym.sym) {
      case SDLK_ESCAPE:
        emu_window.shutdown = true;
        break;
    }
  }
}

static Uint32 gb_palette[4] = {
    0xFFFFFFFF,  // white
    0xAAAAAAFF,  // light gray
    0x555555FF,  // dark gray
    0x000000FF   // black
};

void render_window()
{
  void* tex_pixels;
  int pitch;

  if (SDL_LockTexture(emu_window.texture, NULL, &tex_pixels, &pitch) != 0) {
    SDL_Log("SDL_LockTexture Error: %s", SDL_GetError());
    return;
  }

  Uint32* dst = (Uint32*)tex_pixels;

  for (int y = 0; y < 144; y++) {
    for (int x = 0; x < 160; x++) {
      int index = y * 160 + x;
      uint8_t color_index = framebuffer[index] & 0x03;  // Ensure it's 0–3
      dst[y * (pitch / 4) + x] = gb_palette[color_index];
    }
  }

  SDL_UnlockTexture(emu_window.texture);

  SDL_RenderClear(emu_window.renderer);
  SDL_RenderCopy(emu_window.renderer, emu_window.texture, NULL, NULL);
  SDL_RenderPresent(emu_window.renderer);
}

void terminate_window()
{
  SDL_DestroyTexture(emu_window.texture);
  SDL_DestroyRenderer(emu_window.renderer);
  SDL_DestroyWindow(emu_window.window);
  SDL_Quit();
}
