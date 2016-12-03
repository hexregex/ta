#include <stdio.h>

#include "log.h"
#include "output.h"
#include "communicate.h"

void output(int fd_read, int unused)
{
  while (1)
  {
    printf("output is here\n");
    log_write("output_while-start");
    Comm command;
    comm_recv(fd_read, &command);

    char output[100];
    comm_to_string(&command, output);

    printf("%s\n", output);
    log_write(output);
    log_write("output_while-end");
  }
}
