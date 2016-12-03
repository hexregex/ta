#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "communicate.h"
#include "log.h"

/* Create a pipe.  'read' points to an int which is given the value of the
   file descriptor used to read from the pipe.  'write' points to an int which
   is given the value of the file descriptor used to write to the pipe.
   to the pipe with. */
void comm_connect(int *read, int *write)
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

void comm_send(int pipe, const Comm *command) {
    log_write("comm_send-start");

    if (command == NULL)
    {
      /* TODO: What is best way to handle? */
    }

    write(pipe, command, sizeof(Comm));
    log_write("comm_send-end");
}


void comm_recv(int pipe, Comm *command)
{
    log_write("comm_recv-start");

    read(pipe, command, sizeof(Comm));

    log_write("comm_recv-end");
}

void comm_to_string(const Comm *command, char *string)
{
    sprintf(string, "%c", command->code);
}
