#include "common.h"

static const char* log_type_to_str(log_type type)
{
  switch (type) {
    case lerror:
      return "ERROR";
    case lwarn:
      return "WARN";
    case linfo:
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