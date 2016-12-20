/*  This file (player.h) is part of the ta application.
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
#ifndef AT_PLAYER_TA
#define AT_PLAYER_TA

#include <signal.h>
#include <stdint.h>

#define MAX_FILES 64

/* TODO. Do I really need these globals? */
/* Global value for the current play time in seconds. */
long plr_sec_play_time;

/* Global value for (much more) precise play time in float */
double plr_play_time;

typedef enum
{
    PLAY,
    PAUSE,
    NEXT,
    PREVIOUS,
    REPEAT,
    SEEK,
    RESET
} PlrCode;

typedef struct
{
    uint16_t number;
    char name[264];
    char codec[32];
    uint32_t sample_rate; /* hertz */
    uint16_t duration; /* seconds */
} Track;

typedef struct
{
    /* There used to be other data here.  Leaving as is in case I
     * decide to add more again someday */
    pid_t out_pid;
    const char *file_names[MAX_FILES];
} PlrThreadData;

/* 'plr' == player */
void (*plr_init)();
void (*plr_open)(const char *);
void (*plr_play)();
void (*plr_pause)();
void (*plr_next)();
void (*plr_previous)();
void (*plr_repeat)();
void (*plr_seek)(int);
int (*plr_track_duration)();

void plr_set_track(Track *track,
                   int track_number,
                   const char *track_name,
                   int track_duration);

void *plr_thread_go(void *thread_arg);

#endif /* AT_PLAYER_TA */
