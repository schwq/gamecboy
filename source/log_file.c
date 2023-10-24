#include <log_file.h>
#include <stdio.h>
#include <time.h>

log_context log_ctx;

void init_log(const char* file_name)
{
  log_ctx.log_ptr = fopen(file_name, "w");
  if (log_ctx.log_ptr == NULL) {
    log_ctx.open = false;
    _ERROR("Cannot open log file, you fucked up really bad where! :(");
    // _CRITICAL can abort program or not, log_ctx.open must exist to control writing to file
    // i cant have segmentation fault being thrown here
    _CRITICAL;
  }
  // TODO make some struct to receive current local time, mabye some printf wrapper to make life easy
  time_t rawtime;
  struct tm* timeinfo;
  time(&rawtime);
  timeinfo = localtime(&rawtime);

  log_ctx.open = true;

  fprintf(log_ctx.log_ptr, "last local time logged at last runtime: %s\n",
          asctime(timeinfo));
  // must fflush log file, when the program is aborted, all buffers are discarted and nothing is written to the log file
  // creating a blank file that doesnt help in nothing
  fflush(log_ctx.log_ptr);
}
// nice fprintf wrapper
void write_log(const char* message, ...)
{
  if (log_ctx.open) {
    va_list args;
    va_start(args, message);
    fprintf(log_ctx.log_ptr, message, args);
    fflush(log_ctx.log_ptr);
    va_end(args);
  }
  else {
    _ERROR(
        "cannot write to log if the same isnt opened! stop fuckin around :(");
  }
}

void terminate_log()
{
  log_ctx.open = false;
  fclose(log_ctx.log_ptr);
}

