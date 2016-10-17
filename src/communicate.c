#include <stdio.h>
#include <unistd.h>

static const int BUFFER_SIZE = 100;

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
}

void send_command(int pipe, char *command) {
    FILE * stream;
    stream = fdopen(pipe, "w");
    fputs(command, stream);
    fclose(stream);
}


void recv_command(int pipe, char *command) {
    FILE * stream;
    stream = fdopen(pipe, "r");
    fgets(command, 80, stream);
    fclose(stream);
}
