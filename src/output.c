#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>

#include "ta.h"
#include "output.h"
#include "log.h"
#include "communicate.h"

/* TODO: For dynamic load move this #define elsewhere. */
#define TA_NCURSES
#ifdef TA_NCURSES
#include "ncurses.h"
#endif

static void (*out_init)();
static void (*out_dest)();
static void (*out_track_list)(Track *track_list, int track_count);
static void (*out_track)(Track *track);
static void (*out_play_time_str)(const char *time_str);
static void (*out_operation)(OutCode operation);

static void out_play_time(int seconds)
{
  char time_str[12];
  seconds_to_str(time_str, seconds);
  out_play_time_str(time_str);
}

static inline void out_load_lib() {
    /* TODO: Make load dynamic.  dlsym() */
    out_init = nc_init;
    out_dest = nc_dest;
    out_track_list = nc_track_list;
    out_track = nc_track;
    out_play_time_str = nc_play_time_str;
    out_operation = nc_operation;
}

void seconds_to_str(char *str, int val)
{
    int sec, min;
    sec = val % 60;
    val /= 60;
    min = val % 60;
    val /= 60;

    /* TODO: Improve the output format. */
    if (val > 0)
      sprintf(str, "%2i:%02i:%02i", val, min, sec);
    else
      sprintf(str, "%5i:%02i", min, sec);
}

static inline
void *ta_sig_thread_go()
{
    /* TODO: Select which output command to send. */

    Comm command;
    comm_recv(out_read_from_plr, &command);

    switch((OutCode)command.code)
    {
        case PLAY_TIME:
            out_play_time(command.data.seconds);
            break;
        case TRACK:
            out_track(&command.data.track);
            break;
        default: break;
    }

    return NULL;
}

static inline
void ta_sig_handler()
{
    /* Handle the signal from the player on a different thread. */
    pthread_t sig_thread_id;
    pthread_create(&sig_thread_id, NULL, &ta_sig_thread_go, NULL);
}

static inline
void ta_sig_init()
{
    struct sigaction sa;
    sa.sa_handler = ta_sig_handler;
    sigemptyset(&sa.sa_mask);
    /* Allow functions which error out if interrupted to restart. */
    sa.sa_flags = SA_RESTART;
    sigaction(SIGUSR1, &sa, NULL);
    /* ^ TODO: Find a better way.  Maybe real-time signals. Do something
     * better than highjacking a system signal (SIGURG) which might be used
     * for something else. */
}

void out_process_go()
{
    out_load_lib();
    ta_sig_init();
    out_init();

    Track *track_list = NULL;
    int track_index = 0;
    int track_count = 0;
    int loaded_track_count = 0;

    // int file_flags = fcntl(fd_read, F_GETFL);
    // printf("output flag non-block = %i\n", file_flags);
    while (1)
    {
        Comm command;
        comm_recv(out_read_from_ta, &command);
        log_write_int("output: Command received", command.code);
        switch ((OutCode)command.code)
        {
            case PLAY_TIME:
                log_write_int("Display the play_time", command.data.seconds);
                out_play_time(command.data.seconds);
                break;
            case TRACK:
                log_write("output: TRACK--start");
                /* Output track info. */
                out_track(&command.data.track);
                break;
            case PLAYING:
                out_operation(PLAYING);
                break;
            case PAUSED:
                out_operation(PAUSED);
                break;
            case LOAD_TRACK:
                if (++loaded_track_count <= track_count)
                {
                    log_write("output: LOAD_TRACK");
                    /* Load one more track on the track list. */
                    track_list[track_index++] = command.data.track;
                }
                break;
            case LOAD_TRACK_LIST:
                log_write("output: LOAD_TRACK_LIST--start");
                /* Initiate the loading of the track list. LOAD_TRACK_LIST
                 * should be followed by a series of TRACK commands, one for
                 * each of the tracks to be loaded onto the track list. */
                track_count = command.data.count;
                loaded_track_count = 0;
                track_list = realloc(track_list, track_count * sizeof(Track));
                track_index = 0;
                log_write("output: LOAD_TRACK_LIST--end");
                break;
            case TRACK_LIST:
                out_track_list(track_list, track_count);
                break;
            default: break;
        }

        log_write_int("output: while--end", command.code);
    }

    free(track_list);
    out_dest();
}
