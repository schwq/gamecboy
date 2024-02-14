#include "../include/window.h"
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include "../include/common.h"
#include "graphics.h"
// must be just one window for the whole emulation
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

  SDL_CreateTexture(emu_window.renderer, SDL_PIXELFORMAT_ARGB8888,
                    SDL_TEXTUREACCESS_STATIC, EMULATOR_SCREEN_X,
                    EMULATOR_SCREEN_Y);

  printf("Created SDL2 window!\n");
}

void handle_window_input()
{

  while (SDL_PollEvent(&emu_window.event)) {

    switch (emu_window.event.type) {
      case SDL_QUIT:
        emu_window.shutdown = true;
        break;
    }

    switch (emu_window.event.key.keysym.sym) {
      case SDLK_ESCAPE:
        emu_window.shutdown = true;
        break;
    }
  }
}

void render_window()
{
  emu_window.texture

  SDL_UpdateTexture(emu_window.texture, NULL, sdl2_pixels,
                    EMULATOR_SCREEN_X * sizeof(Uint8));
  SDL_RenderClear(emu_window.renderer);
  SDL_RenderCopy(emu_window.renderer, emu_window.texture, NULL, NULL);
  SDL_RenderPresent(emu_window.renderer);

}

void terminate_window()
{
  SDL_DestroyRenderer(emu_window.renderer);
  SDL_DestroyWindow(emu_window.window);
  SDL_Quit();
}
