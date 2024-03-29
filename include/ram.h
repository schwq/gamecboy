#ifndef _RAM_H_
#define _RAM_H_

#include <wchar.h>
#define HRAM_ADDR_START 0xFF80
#define HRAM_ADDR_END 0xFFFE
#define HRAM_SIZE HRAM_ADDR_END - HRAM_ADDR_START

#define WRAM_ADDR_START 0xC000
#define WRAM_ADDR_END 0xDFFF
#define WRAM_SIZE WRAM_ADDR_END - WRAM_ADDR_START

#define VRAM_START 0x8000
#define VRAM_END 0x9FFF
#define VRAM_SIZE VRAM_END - VRAM_START

#define EXTERNAL_RAM_ADDR_START 0xA000
#define EXTERNAL_RAM_ADDR_END 0xBFFF
#define EXTERNAL_RAM_SIZE EXTERNAL_RAM_ADDR_END - EXTERNAL_RAM_ADDR_START

#define OAM_ADDR_START 0xFE00
#define OAM_ADDR_END 0xFE9F
#define OAM_ADDR_SIZE OAM_ADDR_END - OAM_ADDR_START
#define OAM_DMA_TRANSFER_REG 0xFF46

#include "cartridge.h"

// declares with part of the emulator is accessing the ram
typedef enum {
  kCPU_MEMORY_SCR,
  kPPU_MEMORY_SCR,
  KNONE_MEMORY_SCR
} memory_access_source;

extern memory_access_source access_source;

typedef struct {
  cartridge_context* data;
  u8* ram_data;
} ram_context;

extern ram_context ram;

u8 read_uint8_data(u16 addr);
void write_uint8_data(u16 addr, u8 value);
u8* pointer_to_data_buffer();
uint size_of_data_buffer();
u8* pointer_data_buffer_pos(u16 pos);
u32 read_object_attribute(u16 start_addr);

void print_ram_content();

#endif
