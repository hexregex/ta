#ifndef AT_LOG_TA
#define AT_LOG_TA

#include "communicate.h"

int log_write(const char *log_message);
int log_write_int(const char *log_message, int value);
int log_write_comm(const Comm *command);
int log_write_array_of_strings(const char **abc);

#endif /* AT_LOG_TA */
