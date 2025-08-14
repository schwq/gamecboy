#pragma once

#include <common.h>
#include <gamepad.h>
#include <serial.h>
#include <timer.h>

/**
 * $FF00		DMG	Joypad input
   $FF01	$FF02	DMG	Serial transfer
   $FF04	$FF07	DMG	Timer and divider
   $FF0F		DMG	Interrupts
   $FF10	$FF26	DMG	Audio
   $FF30	$FF3F	DMG	Wave pattern
   $FF40	$FF4B	DMG	LCD Control, Status, Position, Scrolling, and Palettes
   $FF4F		CGB	VRAM Bank Select
   $FF50		DMG	Boot ROM mapping control
   $FF51	$FF55	CGB	VRAM DMA
   $FF68	$FF6B	CGB	BG / OBJ Palettes
   $FF70		CGB	WRAM Bank Select
 */

#define IO_JOYP_P1 0xFF00
#define IO_TIMER_DIV_ADDR_STR 0xFF04
#define IO_TIMER_DIV_ADDR_END 0xFF07
#define IO_IE 0xFF0F
#define IO_AUDIO_ADDR_STR 0xFF10
#define IO_AUDIO_ADDR_END 0xFF3F
#define IO_LCD_ADDR_STR 0xFF40
#define IO_LCD_ADDR_END 0xFF4B

void io_write(u16 addr, u8 value);
u8 io_read(u16 addr);