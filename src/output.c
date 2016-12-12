#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include "ta.h"
#include "output.h"
#include "ncurses.h"
#include "log.h"
#include "communicate.h"

#define USE_NCURSES
#ifndef USE_NCURSES
#include "ncurses.h"
#endif

/* TODO: standard defines this somewhere. */
#define true 1
#define false 0

void out_process_go()
{
    void (*out_init)();
    void (*out_track_list)(Track *track_list);
    void (*out_track)(Track *track);
    void (*out_play_time)(int seconds);
    void (*out_operation)(OutCode operation);
    void (*out_dest)();

    /* TODO: Make load dynamic.  dlsym() */
    out_init = nc_out_init;
    out_track_list = nc_track_list;
    out_track = nc_track;
    out_play_time = nc_play_time;
    out_operation = nc_operation;
    out_dest = nc_dest;

    out_init();

    Track *track_list = NULL;
    int track_index = 0;
    int is_loading_tracks = false;
    int track_count = 0;
    int loaded_track_count = 0;

    // int file_flags = fcntl(fd_read, F_GETFL);
    // printf("output flag non-block = %i\n", file_flags);
    while (1)
    {
        log_write("output_while-start");
        Comm command;
        comm_recv(out_read_from_ta, &command);
        switch ((OutCode)command.code)
        {
            case PLAY_TIME:
                out_play_time(command.data.seconds);
                break;
            case TRACK:
                /* Either attempt to add one more track to the track list or
                 * order track info be displayed. */
                if (is_loading_tracks)
                {
                    if (++loaded_track_count <= track_count)
                        /* Load one more track on the track list. */
                        track_list[track_index++] = command.data.track;
                    else
                        is_loading_tracks = false;
                }
                else /* Output track info. */
                    out_track(&command.data.track);
                break;
            case PLAYING: break;
                out_operation(PLAYING);
            case PAUSED: break;
                out_operation(PAUSED);
            case LOAD_TRACK_LIST:
                /* Initiate the loading of the track list. LOAD_TRACK_LIST
                 * should be followed by a series of TRACK commands, one for
                 * each of the tracks to be loaded onto the track list. */
                is_loading_tracks = true;
                track_count = command.data.count;
                loaded_track_count = 0;
                track_list = realloc(track_list, track_count * sizeof(Track));
                track_index = 0;
                break;
            default: break;
        }

        log_write("output_while-end");
        log_write_comm(&command);
    }

    free(track_list);
    out_dest();
}
