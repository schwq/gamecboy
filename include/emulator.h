#pragma once

#include <common.h>

typedef struct {

  bool paused;
  bool running;
  bool die;
  u64 ticks;

} emu_context;

static emu_context emu;

void emu_cycle(unsigned int cycles);
int emu_run(int argc, char** argv);
