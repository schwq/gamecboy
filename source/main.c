#include "../include/common.h"
#include "../include/cartridge.h"
#include "../include/cpu.h"

bool init_sdl2() {
    bool success = true;
    return success;
}

int main() {
    data_file* data = malloc(sizeof(data_file));
    cartridge_context* cartridge = malloc(sizeof(cartridge_context));
    cartridge->game_data = data; 
    if(!read_data_to_file("source/roms/zelda.gb", cartridge)) {
        printf("Cannot load game!\n");
        return -1;
    }
    cpu_write_reg(reg_bc, 0);
    cpu_write_reg(reg_b, 14);
    cpu_write_reg(reg_c, 10);
    u16 read = cpu_read_reg(reg_bc);
    printf(PRINTF_BINARY_PATTERN_INT16 "\n", PRINTF_BYTE_TO_BINARY_INT16(read));
    printf("[%u]", read);

    u8 x1 = 0b00000001;
    u8 x2 = 0b00010000;
    u16 result = u8_to_u16(x1, x2);

    printf(PRINTF_BINARY_PATTERN_INT16 "\n", PRINTF_BYTE_TO_BINARY_INT16(result));
    // msb      lsb 
    u16 t1 = 0b0000-1000-0100-0000;
    u8 tmsb = msb(t1);
    u8 tlsb = lsb(t1);
    printf("MSB: "PRINTF_BINARY_PATTERN_INT8"\n", PRINTF_BYTE_TO_BINARY_INT8(tmsb));
    printf("LSB: "PRINTF_BINARY_PATTERN_INT8"\n", PRINTF_BYTE_TO_BINARY_INT8(tlsb));

    const char* hello = "hello";
    _ERROR("cannot load %s", hello);

    free(cartridge->header);
    free(cartridge);
    free(data);
    return 0;
}
