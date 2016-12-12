#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <signal.h>
#include <pthread.h>

#include "ta.h"
#include "player.h"
#include "communicate.h"
#include "log.h"

#define USE_FFMPEG
#ifdef USE_FFMPEG
#include "ffmpeg.h"
#endif

#define plr_move_to_next_track() plr_move_to_track(plr_curr_track + 1)
#define plr_move_to_prev_track() plr_move_to_track(plr_curr_track - 1)
#define plr_open_curr_track() plr_open(plr_playlist[plr_curr_track])

/* File descriptors for read write pipes to main. */


static pthread_t ta_thread_id;
static const char **plr_playlist;
static int plr_curr_track;
static int plr_track_count;

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

void plr_pause_sig_handler()
{
    log_write("plr_pause_sig_handler-comm_recv--start");
    Comm command;
    comm_recv(plr_read_from_ta, &command);
    log_write("plr_pause_sig_handler-comm_recv--end");

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
    comm_recv(plr_read_from_ta, &command);
    /* Finally exit this handler function allowing the player thread to resume
       execution where the (previous pause) signal which invoked this handler
       was caught. (Resume audio playback.) */
}

static inline void plr_set_track_count()
{
    int i;
    for (i = 0; plr_playlist[i] != NULL; i++) {}
    plr_track_count = i;
}

static inline void plr_move_to_track(int track)
{
    plr_curr_track = (track >= 0)
        ? ( (track < plr_track_count) ? track : 0 )
        : plr_track_count - 1 ;
}

void plr_other_sig_handler() {

    Comm command;
    comm_recv(plr_read_from_ta, &command);

    /* TODO: When doing these operations I need to flush the buffer containing
       the PCM audio so that little blurbs already read into the buffer before
       this signal is caught will not be played before proceeding to play the
       next or previous track */
    switch (command.code) {
        case PREVIOUS:
            plr_move_to_prev_track();
            plr_open_curr_track();
            break;
        case NEXT:
            plr_move_to_next_track();
            plr_open_curr_track();
            break;
        case SEEK:
            plr_seek(command.data.i);
            break;
    }
}


/* Set up signal handler to catch user signal from main. */
static inline void plr_sig_init()
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


void *plr_time_thread_go()
{
    /* Setting to -1 always forces 0 value draw at beginning. */
    plr_sec_play_time = -1;

    for (;;)
    {
        /* When the the play time value reaches the next whole second
         * signal the main module to update the clock display. */
        if (plr_sec_play_time != (long)plr_play_time)
        {
            pthread_kill(ta_thread_id, SIGURG);
            plr_sec_play_time = (long)plr_play_time;
        }
        /* TODO: Add a delay.  Literally make this thread stop for a short
         * time, otherwise this thread just wastes CPU processing time. */
        /* Maybe give thread lower processing priority? */
    }
}

void *plr_thread_go(void *thread_arg)
{
    log_write_int("player thread id", pthread_self());
    plr_sig_init();

    ta_thread_id = ((PlrThreadData *)thread_arg)->ta_thread_id;
    plr_playlist = ((PlrThreadData *)thread_arg)->file_names;

    /* Default to start playing the first track. */
    plr_curr_track = 0;

    /* Set the number of tracks in the playlist */
    plr_set_track_count();
    log_write_int("track count", plr_track_count);

    log_write_array_of_strings(plr_playlist);

    plr_load_lib();
    plr_init();

    /* These are for testing.
    log_write_int("ta_thread_id in player", ta_thread_id);
    log_write("plr_thread_go--pthread_kill--start");
    int error = pthread_kill(ta_thread_id, SIGURG);
    log_write_int("player pthread_kill error", error); */

    /* TODO: Currently plr_curr_track is the
       actual track number minus one.  I
       should probably fix. */
    /* Spawn thread to keep track of play timer. */
    pthread_t plr_time_thread_id;
    pthread_create(&plr_time_thread_id, NULL, &plr_time_thread_go, NULL);
    log_write("play thread has been spawned");

    while (plr_curr_track < plr_track_count)
    {
        plr_open_curr_track();
        plr_play();
        plr_move_to_next_track();
    }


    /* TODO: What is a useful value to return on thread termination? */
    return NULL;
}
