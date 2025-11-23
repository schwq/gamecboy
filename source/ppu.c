#include <ppu.h>

ppu_context ppu = {0};

void ppu_init();
void ppu_tick();

u8 ppu_read_vram(u16 addr);
void ppu_write_vram(u16 addr, u8 value);

u8 ppu_read_oam(u16 addr);
void ppu_write_oam(u16 addr, u8 value);