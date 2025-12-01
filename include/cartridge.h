#pragma once

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
  cartridge_header* header;
  char name[100];

  u8* rom_data;
  size_t rom_size;

  u8* rom_bankx;
  u8 rom_bank_n;

  u8 ram_bank_n;
  u8* ram_bank;
  u8* banks[16];

  bool save;
  bool battery;

  bool banking_mode;

  bool ram_enable;

} cartridge_context;

extern cartridge_context cartridge;

u8 cart_read(u16 addr);

void cart_write(u16 addr, u8 value);

bool load_rom(const char* file_path);

void cart_write_save();
