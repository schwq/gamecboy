#ifndef _PPU_H_
#define _PPU_H_

#include <common.h>
#include <graphics.h>

#define MINIMUM_MODE3_LENGHT 172

typedef enum {
    kPPU_ACCESS,
    kCPU_ACCESS,
    kNONE_ACCESS
} access_memory_source;

typedef struct {
    access_memory_source source;
} access_video_memory;

void gain_video_memory_access(access_memory_source source);
access_memory_source video_memory_source();

extern int dots_count;


typedef struct {
    access_video_memory video_access_control;
} ppu_context;

extern ppu_context ppu_ctx;

void execute_ppu_mode0();
void execute_ppu_mode1();
void execute_ppu_mode2();
void execute_ppu_mode3();

#endif
