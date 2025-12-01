#include "common.h"

static const char* log_type_to_str(log_type type)
{
  switch (type) {
    case LERROR:
      return "ERROR";
    case LWARN:
      return "WARN";
    case LINFO:
    default:
      return "INFO";
  }
}

void logfmt(log_type type, const char* fmt, ...)
{
  printf("[%s] ", log_type_to_str(type));
  va_list args;
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);
  printf("\n");
}

void* memset_w(void* source, int c, size_t s)
{
  logfmt(LINFO, "Memset %u bytes to the valye %d in address 0x%p", s, c,
         source);
  return memset(source, c, s);
}