#include <SDL2/SDL.h>
#include <gamepad.h>

gamepad_context gamepad_ctx = {0};

gamepad_btn* gamepad_get_buttons()
{
  return &gamepad_ctx.buttons;
}

bool gamepad_sel_dpad()
{
  return gamepad_ctx.sel_dpad;
}

bool gamepad_sel_btn()
{
  return gamepad_ctx.sel_btn;
}

void gamepad_set_sel(u8 value)
{
  gamepad_ctx.sel_btn = value & 0x20;
  gamepad_ctx.sel_dpad = value & 0x10;
}

u8 gamepad_get_output()
{
  u8 output = 0xCF;

  if (!gamepad_sel_btn()) {
    if (gamepad_get_buttons()->start) {
      output &= ~(1 << 3);
    }
    if (gamepad_get_buttons()->select) {
      output &= ~(1 << 2);
    }
    if (gamepad_get_buttons()->a) {
      output &= ~(1 << 0);
    }
    if (gamepad_get_buttons()->b) {
      output &= ~(1 << 1);
    }
  }

  if (!gamepad_sel_dpad()) {
    if (gamepad_get_buttons()->down) {
      output &= ~(1 << 3);
    }
    if (gamepad_get_buttons()->up) {
      output &= ~(1 << 2);
    }
    if (gamepad_get_buttons()->right) {
      output &= ~(1 << 0);
    }
    if (gamepad_get_buttons()->left) {
      output &= ~(1 << 1);
    }
  }
  return output;
}

void gamepad_set_keysym(uint32_t sdlkey, bool state)
{
  switch (sdlkey) {
    case SDLK_DOWN:
      gamepad_get_buttons()->down = state;
    case SDLK_UP:
      gamepad_get_buttons()->up = state;
    case SDLK_RIGHT:
      gamepad_get_buttons()->right = state;
    case SDLK_LEFT:
      gamepad_get_buttons()->left = state;
    case SDLK_x:
      gamepad_get_buttons()->b = state;
    case SDLK_z:
      gamepad_get_buttons()->a = state;
    case SDLK_RETURN:
      gamepad_get_buttons()->start = state;
    case SDLK_TAB:
      gamepad_get_buttons()->select = state;
    default:
      logfmt(lwarn, "Cannot find SDLK_ related to the value: " HEX_PATTERN,
             sdlkey);
  }
}