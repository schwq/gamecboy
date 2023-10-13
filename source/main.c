#include <common.h>
#include <cartridge.h>
#include <cpu.h>
#include <window.h>
#include <log_file.h>
#include <local_time.h>
#include <stdio.h>
#include <graphics.h>

// TODO finish local_time 
int main(int argc, char* argv[]) {
    init_log("gcb_cache.txt");
    init_window(SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC, "GB emulator", 300, 300);
    
    ram.data = malloc(sizeof(cartridge_context));
    ram.data->game_data = malloc(sizeof(data_file));
    if(!read_data_to_file("cpu_instrs.gb", ram.data)) {
        printf("Cannot load game!\n");
        return -1;
    }


    init_cpu();
    while(!emu_window.shutdown){ 
        handle_window_input();
        render_window();            

        if(!cpu.control.shutdown) {
            run_cpu();
        }
    }

    
    
    terminate_window();
    free(ram.data->game_data->buffer_data);
    free(ram.data->game_data);
    free(ram.data->header);
    free(ram.data);
    terminate_log();

   return 0;
}
