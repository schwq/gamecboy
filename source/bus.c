#include <bus.h>
#include <cartridge.h>
#include <cpu.h>
#include <dma.h>
#include <io.h>
#include <ram.h>
/**
 * 0000	3FFF	16 KiB ROM bank 00	From cartridge, usually a fixed bank
   4000	7FFF	16 KiB ROM Bank 01–NN	From cartridge, switchable bank via mapper (if any)
   8000	9FFF	8 KiB Video RAM (VRAM)	In CGB mode, switchable bank 0/1
   A000	BFFF	8 KiB External RAM	From cartridge, switchable bank if any
   C000	CFFF	4 KiB Work RAM (WRAM)	
   D000	DFFF	4 KiB Work RAM (WRAM)	In CGB mode, switchable bank 1–7
   E000	FDFF	Echo RAM (mirror of C000–DDFF)	Nintendo says use of this area is prohibited.
   FE00	FE9F	Object attribute memory (OAM)	
   FEA0	FEFF	Not Usable	Nintendo says use of this area is prohibited.
   FF00	FF7F	I/O Registers	
   FF80	FFFE	High RAM (HRAM)	
   FFFF	FFFF	Interrupt Enable register (IE)	
 */

u8 bus_read(u16 addr)
{
  if (addr < 0x8000) {
    return cart_read(addr);
  }
  else if (addr < 0xA000) {
    //ppu_read_vram
  }
  else if (addr < 0xC000) {
    return cart_read(addr);
  }
  else if (addr < 0xE000) {
    return wram_read(addr);
  }
  else if (addr < 0xFE00) {
    // echo_ram
    return 0x00;
  }
  else if (addr < 0xFEA0) {
    if (dma_is_transferring())
      return 0xFF;
    // oam_read
  }
  else if (addr < 0xFF00) {
    return 0x00;
  }
  else if (addr < 0xFF80) {
    return io_read(addr);
  }
  else if (addr == 0xFFFF) {
    return ie_register_read(addr);
  }
  return hram_read(addr);
}

void bus_write(u16 addr, u8 value)
{
  {
    if (addr < 0x8000) {
      return cart_write(addr, value);
    }
    else if (addr < 0xA000) {
      //ppu_write_vram
    }
    else if (addr < 0xC000) {
      return cart_write(addr, value);
    }
    else if (addr < 0xE000) {
      wram_write(addr, value);
    }
    else if (addr < 0xFE00) {
      // echo_ram
      return 0x00;
    }
    else if (addr < 0xFEA0) {
      if (dma_is_transferring())
        return;
      // oam_write
    }
    else if (addr < 0xFF00) {
      return 0x00;
    }
    else if (addr < 0xFF80) {
      io_write(addr, value);
    }
    else if (addr < 0xFFFE) {
      hram_write(addr, value);
    }
    else if (addr == 0xFFFF) {
      ie_register_write(value);
    }
  }
}