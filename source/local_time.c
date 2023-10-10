#include <local_time.h>

time_context time_ctx;

const char* get_time_str() {
    time(&time_ctx.rawtime);
    time_ctx.timeinfo = localtime(&time_ctx.rawtime);
    return asctime(time_ctx.timeinfo);
}
