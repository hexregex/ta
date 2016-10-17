#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "communicate.h"

static const int BUFFER_SIZE = 100;
static FILE *log;

void connect(int *read, int *write)
{
  /* file descriptors */;
  int fildes[2];
  int status;

  status = pipe(fildes);
  if (status == -1)
  {
    /* TODO */
    /* error creating pipe. handle error */
    return;
  }

  *read = fildes[0];
  *write = fildes[1];

  log_write("**************************");
  log_write_int(*read);
  log_write_int(*write);
}

void send_command(int pipe, char *command) {
    log_write("send_command");
    log_write(command);
    log_write("send_command");

    FILE * stream;
    stream = fdopen(pipe, "w");
    log_write_int(stream);
    log_write("send_command_fprintf");
    fprintf(stream, command);
    log_write("after_fprintf");
    /* if I run this then the next time around stream is assigned null

       from fdopen() call */

    /* fclose(stream); */
}


void recv_command(int pipe, char **command)
{



    log_write("recv_command");


    *command = realloc(*command, 1);
    **command = '\0';

    char *buffer = NULL;

    int read_char;
    FILE *stream;
    stream = fdopen(pipe, "r");
    while ((read_char = fgetc(stream)) != '\n')
    {
      log_write("recv_command_while");
      log_write_int(read_char);
      log_write("after_int");


      /* Space for one character and '\0' */
      int buff_size = strlen(*command) + 2;

      buffer = realloc(buffer, buff_size);
      sprintf(buffer, "%s%c", *command, read_char);
      *command = realloc(*command, buff_size);
      strcpy(*command, buffer);

      log_write(*command);
    }
    log_write("recv_command_after_while");
    fclose(stream);

    free(buffer);
}

int log_write(char *log_message)
{
  int count;
  FILE *log;
  log = fopen("logfile", "a");
  count = fprintf(log, "%s\n", log_message);
  fclose(log);
  return count;
}


int log_write_int(int value)
{
  int count;
  char *log_message;
  log_message = malloc(30);
  sprintf(log_message, "%i", value);
  count = log_write(log_message);
  return count;
}

