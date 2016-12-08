#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <signal.h>

#include "player.h"
#include "ffmpeg.h"
#include "communicate.h"
#include "log.h"


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


void plr_pause_sig_handler(int signo)
{
    printf("player is here. player is here.\n");

    log_write("first comm_recv start");
    Comm command;
    comm_recv(fd_read_from_main, &command);

    int sig; /* Assigned the value of the signal removed by sigwait(). */
    sigset_t ss; /* Signals to wait for, in this case assigned to SIGUSR1. */
    sigemptyset(&ss);
    sigaddset(&ss, SIGUSR1);

    /*******************************************************
     * Wait for another pause signal then resume playback. *
     *******************************************************/
    /* Wait here until another pause signal is pending (it will be blocked),
       then remove the pause signal from the list of pending signals (so this
       handler function won't be called again immediately on exit). */
    sigwait(&ss, &sig);
    log_write("second comm_recv start");
    /* Next remove from the pipe the command which was sent before the signal
       just removed was generated (both by main). */
    comm_recv(fd_read_from_main, &command);
    /* Finally exit this handler function allowing the player thread to resume
       execution where the (previous pause) signal which invoked this handler
       was caught. (Resume audio playback.) */
}

void plr_other_sig_handler(int signo) {

    Comm command;
    comm_recv(fd_read_from_main, &command);


}

/* Set up signal handler to catch user signal from main. */
void plr_sig_init()
{
    struct sigaction sa_pause;
    sa_pause.sa_handler = plr_pause_sig_handler;
    /* Block no signals (except SIGUSR1) while the signal handler is running. */
    sigemptyset(&sa_pause.sa_mask);
    /* Allow functions which error out if interrupted to restart. */
    sa_pause.sa_flags = SA_RESTART;
    sigaction(SIGUSR1, &sa_pause, NULL);

    struct sigaction sa_other;
    sa_other.sa_handler = plr_other_sig_handler;
    /* Block all signals while the signal handler is running. */
    sigfillset(&sa_other.sa_mask);
    sa_other.sa_flags = SA_RESTART;
    sigaction(SIGUSR2, &sa_other, NULL);
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
