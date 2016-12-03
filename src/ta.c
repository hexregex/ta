#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>

#include "ui.h"
#include "player.h"
#include "input.h"
#include "output.h"
#include "communicate.h"
#include "log.h"


#define USE_FFMPEG
#ifdef USE_FFMPEG
#include "ffmpeg.h"
#endif

pid_t fork_me( void (*go)(int, int), int fd_read, int fd_write )
{
    pid_t pid = fork();
    if (pid == 0)
    {
        /* Executing the child process. */
        go(fd_read, fd_write);
        /* TODO: What should the child process return? */
        /* It shouldn't matter because the 'go' functions
          should only return on program termination. */
        return 0;
    }
    else
        /* Executing the parent process. */
        return pid;

    /* TODO: Add error checking if process cannot be forked */
    /* pid returned to parent would be -1 so check errno */
}

int main (int argc, char* argv[])
{
    /* Fork media player module process. */
    /* TODO: add two pipes and 4 file descriptors for this */
    /* pid_t player_pid = */
    fork_me(&player, -1, -1);

    /* init_ui(); */

    /* 'fd' == file descriptor */
    /* 'in' == input, 'out' == output */
    int fd_in_write; /* fd written to by input module. */
    int fd_main_read; /* fd read by main module. */
    int fd_main_write; /* fd written to by main module. */
    int fd_out_read; /* fd read by output module. */

    comm_connect(&fd_main_read, &fd_in_write);
    /* Fork input module process. */
    /* pid_t input_pid = */
    fork_me(&input, -1,  fd_in_write);

    comm_connect(&fd_out_read, &fd_main_write);
    /* Fork output module process. */
    /* pid_t output_pid = */
    fork_me(&output, fd_out_read, -1);

    Comm command;
    while (1)
    {

        /* printf("main is here\n"); */
        log_write("main_while_start");
        comm_recv(fd_main_read, &command);
        /* TODO: Add all the logic to communicate with player. */
        /* For now just echo command from input to output. */
        comm_send(fd_main_write, &command);

        /* TODO: Terminate the while loop when a specific key is pressed. */
    }
}
