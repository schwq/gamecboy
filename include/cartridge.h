#ifndef _CARTRIDGE_H_
#define _CARTRIDGE_H_

#include "common.h"

typedef struct {
    u8 entry[4];
    u8 logo[0x30];
    char title[16];
    u16 new_lic_code;
    u8 sgb_flag;
    u8 type;
    u8 rom_size;
    u8 ram_size;
    u8 dest_code;
    u8 lic_code;
    u8 version;
    u8 checksum;
    u16 global_checksum;
} cartridge_header;

typedef struct {
    u8* buffer_data;
    u32 buffer_size;
} data_file;

typedef struct {
    data_file* game_data;
    cartridge_header* header;
    char name[100];
} cartridge_context;

bool read_data_to_file(const char* file_path, cartridge_context* cart);



#endif