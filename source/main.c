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
    if(!read_data_to_file("source/roms/zelda.gb", ram.data)) {
        printf("Cannot load game!\n");
        return -1;
    }

    u8 bytes[16] = {0x3C, 0x7E, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x7E, 0x5E, 0x7E, 0x0A, 0x7C, 0x56, 0x38, 0x7C};
    graphics_tile test_t = convert_memory_to_tile(bytes);
    FILE* graphics_file = fopen("graphics.txt", "wb");
    if(graphics_file == NULL) {
        _ERROR("cannot open graphics file!\n");
    }
    int flag = 0;
    flag = fwrite(&test_t, sizeof(graphics_file), 1, graphics_file);
    if(flag) {
        printf("Success in writing to graphics!\n");
    } else {
        _ERROR("cannot write to graphics!\n");
    }
    fflush(graphics_file);
    fclose(graphics_file);

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
