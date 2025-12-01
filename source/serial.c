#include <serial.h>

serial_contenxt serial;

u8 serial_read(u16 addr)
{
  if (addr == SERIAL_SB)
    return serial.data[0];
  if (addr == SERIAL_SC)
    return serial.data[1];

  logfmt(LERROR, "Address is out of range to read serial data! " HEX_PATTERN,
         addr);

  return 0xFF;
}

void serial_write(u16 addr, u8 value)
{
  if (addr == SERIAL_SB) {
    serial.data[0] = value;
    return;
  }
  if (addr == SERIAL_SC) {
    serial.data[1] = value;
    return;
  }

  logfmt(LERROR, "Address is out of range to write serial data! " HEX_PATTERN,
         addr);
}