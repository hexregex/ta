#include <stdio.h>
#include <stdlib.h>

/* Write a string to the log file. */
int log_write(char *log_message)
{
  int count;
  FILE *log;
  log = fopen("logfile", "a");
  count = fprintf(log, "%s\n", log_message);
  fclose(log);
  return count;
}


/* Convert an int to a string and write to the log file. */
int log_write_int(int value)
{
  int count;
  char *log_message;
  log_message = malloc(30);
  sprintf(log_message, "%i", value);
  count = log_write(log_message);
  return count;
}
