#include <cpu.h>
#include <io.h>

u8 io_read(u16 addr)
{
  if (addr == IO_JOYP_P1) {
    return gamepad_get_output();
  }
  else if (BETWEEN(addr, SERIAL_SB, SERIAL_SC)) {
    return serial_read(addr);
  }
  else if (BETWEEN(addr, IO_TIMER_DIV_ADDR_STR, IO_TIMER_DIV_ADDR_END)) {
    return timer_read(addr);
  }
  else if (addr == IO_IE) {
    return ie_flags_read();
  }
  else if (BETWEEN(addr, IO_AUDIO_ADDR_STR, IO_AUDIO_ADDR_END)) {
    // return read_audio();
  }
  else if (BETWEEN(addr, IO_LCD_ADDR_STR, IO_LCD_ADDR_END)) {
    //return read_lcd();
  }

  logfmt(lerror, "IO read out of range " HEX_PATTERN, addr);
  return 0x00;
}

void io_write(u16 addr, u8 value)
{
  if (addr == IO_JOYP_P1) {
    gamepad_set_sel(value);
    return;
  }
  else if (BETWEEN(addr, SERIAL_SB, SERIAL_SC)) {
    serial_write(addr, value);
    return;
  }
  else if (BETWEEN(addr, IO_TIMER_DIV_ADDR_STR, IO_TIMER_DIV_ADDR_END)) {
    timer_write(addr, value);
    return;
  }
  else if (addr == IO_IE) {
    ie_flags_write(value);
    return;
  }
  else if (BETWEEN(addr, IO_AUDIO_ADDR_STR, IO_AUDIO_ADDR_END)) {
    // return write_audio();
  }
  else if (BETWEEN(addr, IO_LCD_ADDR_STR, IO_LCD_ADDR_END)) {
    //return write_lcd();
  }

  logfmt(lerror, "IO write out of range " HEX_PATTERN, addr);
}