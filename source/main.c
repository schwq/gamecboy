#include "../include/common.h"
#include "../include/cartridge.h"
#include "../include/cpu.h"

int main() {
    ram.data = malloc(sizeof(cartridge_context));
    ram.data->game_data = malloc(sizeof(data_file));
    if(!read_data_to_file("source/roms/zelda.gb", ram.data)) {
        printf("Cannot load game!\n");
        return -1;
    }
   
    init_cpu();
    run_cpu();

    free(ram.data->game_data->buffer_data);
    free(ram.data->game_data);
    free(ram.data->header);
    free(ram.data);

    return 0;
}
