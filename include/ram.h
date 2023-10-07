#ifndef _RAM_H_
#define _RAM_H_

#include "cartridge.h"

typedef struct {
    cartridge_context* data;
} ram_context;

extern ram_context ram;

u8 read_uint8_data(u16 addr);
void write_uint8_data(u16 addr, u8 value);
u8* pointer_to_data_buffer();
uint size_of_data_buffer();

#endif