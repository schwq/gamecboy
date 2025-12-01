#include <timer.h>

timer_context timer = {0};

void timer_init()
{
  timer.div = 0xAC00;
}

static inline bool timer_tac_enable()
{
  return timer.tac & (1 << 2);
}

static inline u8 timer_clock_select()
{
  return timer.tac & 0b11;
}

static inline bool timer_update(u8 shift)
{
  return (timer.div & (1 << shift)) && (!(timer.div & (1 << shift)));
}

void timer_tick()
{

  u16 prev = timer.div;
  timer.div++;

  bool update = false;

  switch (timer_clock_select()) {
    case 0b00:
      update = timer_update(9);
      break;
    case 0b01:
      update = timer_update(3);
      break;
    case 0b10:
      update = timer_update(5);
      break;
    case 0b11:
      update = timer_update(7);
      break;
  }

  if (update && timer_tac_enable()) {
    timer.tima++;
    if (timer.tima == 0xFF) {
      timer.tima = timer.tma;
      // ei_timer();
    }
  }
}

void timer_write(u16 addr, u8 value)
{
  switch (addr) {
    case TIMER_DIV_ADDR:
      timer.div = 0x00;
      break;
    case TIMER_TIMA_ADDR:
      timer.tima = value;
      break;
    case TIMER_TMA_ADDR:
      timer.tma = value;
      break;
    case TIMER_TAC_ADDR:
      timer.tac = value;
      break;
    default:
      logfmt(LERROR, "Timer write out of range " HEX_PATTERN, addr);
  }
}

u8 timer_read(u16 addr)
{
  switch (addr) {
    case TIMER_DIV_ADDR:
      return (timer.div >> 8);
    case TIMER_TIMA_ADDR:
      return timer.tima;
    case TIMER_TMA_ADDR:
      return timer.tma;
    case TIMER_TAC_ADDR:
      return timer.tac;
    default:
      logfmt(LERROR, "Timer read out of range " HEX_PATTERN, addr);
      return 0xFF;
  }
}