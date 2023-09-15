#include "../include/ram.h"

u8 read_uint8_data(u16 addr) {
    // todo implement ram 
    if(addr > ram.data.game_data->buffer_size) {
        _ERROR("ram_read_addr_out_of_range_error!\n");
    } else {
        return ram.data.game_data->buffer_data[addr];
    }
}

void write_uint8_data(u16 addr, u8 value) {
    if(addr > ram.data.game_data->buffer_size) {
        _ERROR("ram_write_addr_out_of_range_error!\n");
    } else {
        ram.data.game_data->buffer_data[addr] = value;
    }
}

u8* pointer_to_data_buffer() {
    return &ram.data.game_data->buffer_data;
}

uint size_of_data_buffer() {
    return ram.data.game_data->buffer_size;
}