#include <dma.h>
#include <ppu.h>

void dma_start(u8 value)
{
  dma.active = true;
  dma.byte = 0;
  dma.delay = 2;
  dma.value = value;
}

void dma_tick()
{
  if (!dma.active)
    return;
  if (dma.delay) {
    dma.delay--;
    return;
  }
  ppu_write_oam(dma.byte, bus_read((dma.value * 0x100) + dma.byte));
  dma.byte++;
  dma.active = dma.byte < 0xA0;
}

bool dma_is_transferring()
{
  return dma.active;
}