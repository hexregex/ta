#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "communicate.h"
#include "log.h"

static const int BUFFER_SIZE = 100;
static FILE *log;

/* Create a pipe.  'read' points to an int which is given the value of the
   file descriptor used to read from the pipe.  'write' points to an int which
   is given the value of the file descriptor used to write to the pipe.
   to the pipe with. */
void make_connection(int *read, int *write)
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
    /*
    log_write("send_command start");
    log_write(command);
    log_write("send_command end");
    */

    FILE * stream;
    stream = fdopen(pipe, "w");
    fprintf(stream, command);
    /*log_write("after_fprintf"); */

    /* TODO: Figure this out. */
    /* if I run this then the next time around stream is assigned null

       from fdopen() call */

    /* fclose(stream); */
}


/* pass a pointer to a string because when I was just passing a string
   realloc would change the address stored in the character pointer
   so command from the calling routing (who's address pointed to remained
   unchanged) would end up pointing to unallocated memory and seg fault. */
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
