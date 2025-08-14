#include <cartridge.h>
#include <common.h>
#include <cpu.h>
#include <graphics.h>

#include <ppu.h>
#include <stdio.h>
#include <stdlib.h>
#include <window.h>

int main(int argc, char* argv[])
{
  if (argc == 1) {
    logfmt(lwarn, "You must declared a rom path!");
    return EXIT_FAILURE;
  }

  init_window(SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC,
              "GB emulator", 300, 300);
  init_ppu();

  if (argc == 2) {
    if (!load_rom(argv[1])) {
      logfmt(lerror, "Cannot load game!\n");
      return EXIT_FAILURE;
    }
    logfmt(linfo, "Loaded game : %s\n", argv[1]);
  }

  init_cpu();
  while (!emu_window.shutdown) {
    handle_window_input();
    render_window();

    if (!cpu.control.shutdown) {
      run_cpu();
    }

    //draw_ppu();
  }
  terminate_ppu();
  terminate_window();
  return EXIT_SUCCESS;
}
