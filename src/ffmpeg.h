/*  This file (ffmpeg.h) is part of the ta application.
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
#ifndef AT_FFMPEG_TA
#define AT_FFMPEG_TA

void ff_init();
void ff_dest();

void ff_open(const char* in_filename);
void ff_play();
void ff_pause();
void ff_next();
void ff_previous();
void ff_repeat();
void ff_seek();

int ff_track_duration();

#endif /* AT_FFMPEG_TA */
