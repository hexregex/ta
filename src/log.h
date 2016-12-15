#ifndef AT_LOG_TA
#define AT_LOG_TA

#include "communicate.h"

void log_write(const char *log_message);
void log_write_int(const char *log_message, int value);
void log_write_comm(const Comm *command);
void log_write_array_of_strings(const char **abc);

#endif /* AT_LOG_TA */
