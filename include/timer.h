#pragma once

#include "common.h"

#define TIMER_DIV_ADDR 0xFF04
#define TIMER_TIMA_ADDR 0xFF05
#define TIMER_TMA_ADDR 0xFF06
#define TIMER_TAC_ADDR 0xFF07

typedef struct {

  u16 div;
  u8 tima;
  u8 tma;
  u8 tac;

} timer_context;

extern timer_context timer;

void timer_init();

void timer_tick();

void timer_write(u16 addr, u8 value);

u8 timer_read(u16 addr);