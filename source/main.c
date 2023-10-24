#include <cartridge.h>
#include <common.h>
#include <cpu.h>
#include <graphics.h>
#include <local_time.h>
#include <log_file.h>
#include <ppu.h>
#include <stdio.h>
#include <window.h>

int main(int argc, char* argv[])
{
  init_log("gcb_cache.txt");
  init_window(SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC,
              "GB emulator", 300, 300);
  init_ppu();

  ram.data = malloc(sizeof(cartridge_context));
  ram.data->game_data = malloc(sizeof(data_file));
  if (!read_data_to_file("cpu_instrs.gb", ram.data)) {
    printf("Cannot load game!\n");
    return -1;
  }

  init_cpu();
  while (!emu_window.shutdown) {
    handle_window_input();
    render_window();

    if (!cpu.control.shutdown) {
      run_cpu();
    }

    draw_ppu();
  }
  terminate_ppu();
  terminate_window();
  free(ram.data->game_data->buffer_data);
  free(ram.data->game_data);
  free(ram.data->header);
  free(ram.data);
  terminate_log();

  return 0;
}
