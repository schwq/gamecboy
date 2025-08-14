#pragma once

#include <common.h>

// FF01 — SB: Serial transfer data
#define SERIAL_SB 0xFF01

//FF02 — SC: Serial transfer control
#define SERIAL_SC 0xFF02

typedef struct {

  u8 data[2];

} serial_contenxt;

extern serial_contenxt serial;

u8 serial_read(u16 addr);
void serial_write(u16 addr, u8 value);

// TODO: Make two gameboys communicate!