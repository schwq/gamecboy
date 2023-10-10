#ifndef _LOG_FILE_H_
#define _LOG_FILE_H_

#include <common.h>

typedef struct {
    FILE* log_ptr;
    bool open;
} log_context;

extern log_context log_ctx;

void init_log(const char* file_name);
void write_log(const char* message, ...);
void terminate_log();

#endif
