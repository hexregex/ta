#ifndef AT_LOG_TA
#define AT_LOG_TA

#include "communicate.h"

int log_write(const char *log_message);
int log_write_int(int value);
int log_write_comm(const Comm *command);

#endif /* AT_LOG_TA */
