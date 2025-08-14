#pragma once

#include "common.h"

typedef struct {

  bool start;
  bool select;
  bool a;
  bool b;
  bool up;
  bool down;
  bool left;
  bool right;

} gamepad_btn;

typedef struct {

  gamepad_btn buttons;
  bool sel_dpad;
  bool sel_btn;

} gamepad_context;

extern gamepad_context gamepad_ctx;

gamepad_btn* gamepad_get_buttons();
void gamepad_set_keysym(uint32_t sdlkey, bool state);

bool gamepad_sel_dpad();
bool gamepad_sel_btn();
void gamepad_set_sel(u8 value);
u8 gamepad_get_output();