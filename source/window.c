#include <gamepad.h>
#include <window.h>

window_context window = {0};

int window_init(const char* title)
{

  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    logfmt(lerror, "Cannot initialize SDL2! %s", SDL_GetError());
    return -1;
  }

  window.window =
      SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       LCD_WIDTH, LCD_HEIGHT, SDL_WINDOW_SHOWN);

  if (!window.window) {
    logfmt(lerror, "Cannot initialize SDL2 window! %s", SDL_GetError());
    SDL_Quit();
    return -1;
  }

  window.renderer =
      SDL_CreateRenderer(window.window, -1, SDL_RENDERER_ACCELERATED);

  if (!window.renderer) {
    logfmt(lerror, "Cannot initialize SDL2 renderer! %s", SDL_GetError());
    SDL_Quit();
    return -1;
  }

  window.texture =
      SDL_CreateTexture(window.renderer, SDL_PIXELFORMAT_ARGB8888,
                        SDL_TEXTUREACCESS_STREAMING, LCD_WIDTH, LCD_HEIGHT);
  window.running = true;

  window.framebuffer = (u32*)calloc(FRAMEBUFFER_SIZE, sizeof(u32));
  memset_w(window.framebuffer, 0xFF, FRAMEBUFFER_SIZE * sizeof(u32));
}

void window_run()
{
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT)
      window.running = false;

    if (event.type == SDL_KEYDOWN)
      gamepad_set_keysym(event.key.keysym.sym, true);
    if (event.type == SDL_KEYUP)
      gamepad_set_keysym(event.key.keysym.sym, false);
  }

  window.frame++;

  SDL_UpdateTexture(window.texture, NULL, window.framebuffer,
                    sizeof(u32) * LCD_WIDTH);
  SDL_RenderClear(window.renderer);
  SDL_RenderCopy(window.renderer, window.texture, NULL, NULL);
  SDL_RenderPresent(window.renderer);
}

void window_terminate()
{
  FREE(window.framebuffer);
  FREE_FUN(window.texture, SDL_DestroyTexture);
  FREE_FUN(window.renderer, SDL_DestroyRenderer);
  FREE_FUN(window.window, SDL_DestroyWindow);
  SDL_Quit();
}