#pragma once

#include "common.h"

typedef enum {

  INT_VBLANK,
  INT_LCD,
  INT_TIMER,
  INT_SERIAL,
  INT_JOYPAD

} interrupt_type;

typedef struct {

  u8 ime;
  u8 ie;

} interrupt_context;

extern interrupt_context interrupt;