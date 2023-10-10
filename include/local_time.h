#ifndef _LOCAL_TIME_H_
#define _LOCAL_TIME_H_

#include <common.h>

// struct for handling the emulator timing, and local time 
// TODO implement emulator clock on this file, can be helpful for cpu cycles
typedef struct {
    time_t rawtime;
    struct tm * timeinfo;
} time_context;

extern time_context time_ctx;

//void init_time();
const char* get_time_str();

#endif
