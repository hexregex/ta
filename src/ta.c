/*  This file (ta.c) is part of the ta application.
    Copyright (C) 2016-2017 Aaron Calder

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>

#include "ta.h"
#include "player.h"
#include "input.h"
#include "output.h"
#include "communicate.h"
#include "log.h"

/* Macros for signaling player based on keypress. */
#define ta_either_keypress(in_code_1, in_code_2) \
    (ta_keypress(in_code_1) || ta_keypress(in_code_2))

#define ta_keypress(in_code) \
    ((InCode)command.code == in_code)

#define ta_signal_player(play_comm, signal) \
do                                           \
{                                             \
    Comm command2;                             \
    command2.code = play_comm;                  \
    comm_send(ta_write_to_plr, &command2);       \
    pthread_kill(plr_thread_id, signal);          \
}                                                  \
while (0)
/* ^ Do-while swallows trailing semicolon, preventing extra null statment. */


static inline
pid_t ta_fork_me(void (*go)())
{
    pid_t pid = fork();
    if (pid == 0)
    {   /* Executing the child process. */
        go();
        /* TODO: What should the child process return?
         * Does it matter what the child process returns? */
        return 0;
    }
    else /* Executing the parent process. */
        return pid;
    /* TODO: Add error checking if process cannot be forked.
     * pid returned to parent would be -1 so check errno     */
}


/* Shutdown everything and cleanup. */
static inline
void ta_dest(pid_t in_pid, pid_t out_pid, pthread_t plr_thread_id)
{
    /* TODO: send catchable signals to input, output, and player, and make
     * them terminate gracefully. */

    /* kill input process */
    kill(in_pid, SIGKILL);
    /* kill output process */
    kill (out_pid, SIGKILL);
    /* terminate the player thread. */
    pthread_kill(plr_thread_id, SIGKILL);
    /* TODO: Do any other program cleanup. */
    log_write("Made it to the end here.");
}

static inline
void ta_signal_player_seek(pthread_t pt_id, int seconds)
{
    Comm command;
    command.code = SEEK;
    command.data.seconds = seconds;
    comm_send(ta_write_to_plr, &command);
    pthread_kill(pt_id, SIGUSR2);
}

int main (int argc, char **argv)
{
    if (argc <= 1)
    {   /* No file names input. Nothing to play. Terminate.*/
        /* TODO: Output command usage info to terminal before terminating. */
        exit(1);
    }

    /* Create pipe from input to main then fork input process. */
    comm_connect(&ta_read_from_in, &in_write_to_ta);
    pid_t in_pid = ta_fork_me(&in_process_go);

    log_write_int("input forked. pid=", in_pid);

    /* Create pipe from main to output. */
    comm_connect(&out_read_from_ta, &ta_write_to_out);
    /* Create pipe from player to output. */
    comm_connect(&out_read_from_plr, &plr_write_to_out);
    /* Fork output process */
    pid_t out_pid = ta_fork_me(&out_process_go);

    log_write_int("output forked. pid=", out_pid);

    /* Create pipe from main to player. */
    comm_connect(&plr_read_from_ta, &ta_write_to_plr);

    /* TODO: Free up file descriptors which are not used by this process. */



    /* Pack data to send to the player thread. */
    PlrThreadData plr_thread_data = { out_pid, {NULL} };
    /* TODO: Quick and dirty, improve this section. */
    /* TODO: Verify that the files input are audio files.  Play only the
     * audio files and ignore the rest. */
    int i;
    for (i = 0; i < MAX_FILES; i++)
    {
        /* TODO: find out if array elements which are pointers are
           automatically initialized to NULL or not. */
        /* argv[1] is the first command line argument. */
        plr_thread_data.file_names[i] = (i < argc - 1) ? argv[i + 1] : NULL;
    }

    /* Spawn player thread. */
    pthread_t plr_thread_id;
    pthread_create(&plr_thread_id,
                   NULL,
                   &plr_thread_go,
                   (void *)&plr_thread_data);

    /* Load the track list. */
    Comm command;
    command.code = LOAD_TRACK_LIST;
    command.data.count = argc - 1;
    comm_send(ta_write_to_out, &command);

    for (i = 1; i < argc; i++)
    {
        command.code = LOAD_TRACK;
        plr_set_track(&command.data.track, i, argv[i], 0);
        comm_send(ta_write_to_out, &command);
    }

    command.code = TRACK_LIST;
    comm_send(ta_write_to_out, &command);

    while (1)
    {
        log_write("main_while_start");
        comm_recv(ta_read_from_in, &command);

        log_write_int("Input command received by main",
                      (InCode)command.code);

        /* TODO: Improve this whole selection and redirection process. */
        /* TODO: Make keypress to operation mappings user customizable. */
        if ta_either_keypress(Q, ESC)
        {   /* User quit the program. */
            ta_dest(in_pid, out_pid, plr_thread_id);
            break;
        }
        else if ta_keypress(SPACE) /* pause or resume */
            ta_signal_player(PAUSE, SIGUSR1);
        else if ta_either_keypress(K, UP)
            ta_signal_player(PREVIOUS, SIGUSR2);
        else if ta_either_keypress(J, DOWN)
            ta_signal_player(NEXT, SIGUSR2);
        else if ta_either_keypress(H, LEFT)
            ta_signal_player_seek(plr_thread_id, -10);
        else if ta_either_keypress(L, RIGHT)
            ta_signal_player_seek(plr_thread_id, 10);
    }

    /* Holy explicit Batman! */
    exit(0);
    /* TODO: Return some useful exit codes. */
}
