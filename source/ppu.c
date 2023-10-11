#include <ppu.h>

void gain_video_memory_access(access_memory_source source) {
    ppu_ctx.video_access_control.source = source;
}
access_memory_source video_memory_source() {
    return ppu_ctx.video_access_control.source;
}


