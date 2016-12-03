#include <stdio.h>
#include <stdlib.h>
#include "communicate.h"

/* Write a string to the log file. */
int log_write(const char *log_message)
{
  int count;
  FILE *log;
  log = fopen("logfile", "a");
  count = fprintf(log, "%s\n", log_message);
  //count = fprintf(stderr, "%s\n", log_message);
  fclose(log);
  return count;
}

/* Convert an int to a string and write to the log file. */
int log_write_int(int value)
{
    char log_message[30];
    sprintf(log_message, "%i", value);
    return log_write(log_message);
}

int log_write_comm(const Comm *command)
{

    char comm_string[80];
    comm_to_string(command, comm_string);
    return log_write(comm_string);
}

