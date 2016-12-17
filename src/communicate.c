#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "communicate.h"
#include "log.h"

/* Create a pipe.  'read' points to an int which is given the value of the
 * file descriptor used to read from the pipe.  'write' points to an int which
 * is given the value of the file descriptor used to write to the pipe.
 * to the pipe with. */
void comm_connect(int *fd_read, int *fd_write)
{
    /* file descriptors */;
    int fildes[2];
    int status;

    status = pipe(fildes);
    if (status == -1)
        /* TODO: Error creating pipe. handle error */
        return;

    *fd_read = fildes[0];
    *fd_write = fildes[1];

    log_write("**************************");
    log_write_int("", *fd_read);
    log_write_int("", *fd_write);
    log_write("**************************");
}

void comm_send(int pipe, const Comm *command) {
    log_write_int("comm_send-start", pipe);

    if (command == NULL)
    {
        log_write(">>>>>>>>>>>>>>>>>>>>>>ALERT<<<<<<<<<<<<<<<<<<<<<<\ncomm_send() received NULL value for Comm *command.");
        return;
    }

    write(pipe, command, sizeof(Comm));

    log_write_int("comm_send-end", pipe);
}

void comm_recv(int pipe, Comm *command)
{
    log_write_int("comm_recv-start", pipe);

    read(pipe, command, sizeof(Comm));

    log_write_int("comm_recv-end", pipe);
}

/* Gee this is looking more and more OO like. */
void comm_to_string(const Comm *command, char *string)
{
    sprintf(string, "%i", command->code);
}
