#include "../include/common.h"
#include "../include/cartridge.h"
#include "../include/cpu.h"
#include "../include/window.h"


int main(int argc, char* argv[]) {

    init_window(SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC, "GB emulator", 300, 300);

    ram.data = malloc(sizeof(cartridge_context));
    ram.data->game_data = malloc(sizeof(data_file));
    if(!read_data_to_file("source/roms/zelda.gb", ram.data)) {
        printf("Cannot load game!\n");
        return -1;
    }

    init_cpu();
    while(!emu_window.shutdown || !cpu.control.shutdown){ 
        handle_window_input();
        render_window();            
        //run_cpu();
    }
    
    terminate_window();

    free(ram.data->game_data->buffer_data);
    free(ram.data->game_data);
    free(ram.data->header);
    free(ram.data);

   return 0;
}
