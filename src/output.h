/*  This file (output.h) is part of the ta application.
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
#ifndef AT_OUTPUT_TA
#define AT_OUTPUT_TA

/* TODO: ISO C defines these somewhere. */
#define true 1
#define false 0

typedef enum
{
    PLAYING,
    PAUSED,
    TRACK,
    LOAD_TRACK_LIST,
    LOAD_TRACK,
    PLAY_TIME,
    TRACK_LIST
} OutCode;

void seconds_to_str(char *str, int val);

void out_process_go();

#endif /* AT_OUTPUT_TA */
