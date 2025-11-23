#pragma once

#include "common.h"

typedef enum {

  INT_VBLANK = 1,
  INT_LCD = 2,
  INT_TIMER = 4,
  INT_SERIAL = 8,
  INT_JOYPAD = 16

} interrupt_type;

void int_handle_interrupt();