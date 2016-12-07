#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <signal.h>

#include "player.h"
#include "ffmpeg.h"
#include "communicate.h"


/* File descriptors for read write pipes to main. */
static int fd_read_from_main;
static int fd_write_to_main;

static inline void plr_load_lib() {
    /* TODO: Load the functions pointed to dynamically. */
    /* Currently defined in ffmpeg.c */
    plr_init = ff_init;
    plr_open = ff_open;
    plr_play = ff_play;
    plr_pause = ff_pause;
    plr_next = ff_next;
    plr_previous = ff_previous;
    plr_repeat = ff_repeat;
    plr_seek = ff_seek;
}

void plr_sig_handler(int signo)
{
    printf("player is here. player is here.\n");

    Comm command;
    comm_recv(fd_read_from_main, &command);

    printf("%i", command.code);

    switch (command.code)
    {
        case PLAY: plr_play(); break;
        case PAUSE: plr_pause(); break;
    }
}

/* Set up signal handler to catch user signal from main. */
void plr_sig_init()
{
    struct sigaction sa;
    sa.sa_handler = plr_sig_handler;
    /* Block all signals while the signal handler is running. */
    sigfillset(&sa.sa_mask);
    /* Allow functions which error out if interrupted to restart. */
    sa.sa_flags = SA_RESTART;
    sigaction(SIGUSR1, &sa, NULL);
}

void *plr_thread_go(void *thread_arg)
{
    plr_sig_init();

    fd_read_from_main = ((PlrThreadData *)thread_arg)->fd_read;
    fd_write_to_main = ((PlrThreadData *)thread_arg)->fd_write;
    char **in_filename = ((PlrThreadData *)thread_arg)->file_names;

    plr_load_lib();

    plr_init();
    plr_open(in_filename[1]);
    plr_play();

    /* TODO: What is a useful value to return on thread termination? */
    return NULL;
}
