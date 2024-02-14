#include "../include/ram.h"
#include "graphics.h"

ram_context ram;

u8 read_uint8_data(u16 addr)
{
  // todo implement ram
  if (addr > 0xffff) {
    _ERROR("ram_read_addr_out_of_range_error!\n");
  }
  else {
    return ram.ram_data[addr];
  }
  return 0;
}

void write_uint8_data(u16 addr, u8 value)
{
  if (addr > 0xffff) {
    _ERROR("ram_write_addr_out_of_range_error! Addr " HEX_PATTERN "\n", addr);
  }

  else {
    ram.ram_data[addr] = value;
  }
}

uint size_of_data_buffer()
{
  return ram.data->game_data->buffer_size;
}

void print_ram_content() {
  printf("Printing content of ram\n");
  for(unsigned int x = 0x0100; x < 0x0110; x++) {
     
      
    printf(HEX_PATTERN " ", ram.ram_data[x]);
  }
}
