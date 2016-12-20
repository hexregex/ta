/*  This file (communicate.h) is part of the ta application.
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
#ifndef AT_COMMUNICATE_TA
#define AT_COMMUNICATE_TA

#include "player.h"

/* This is the data type that the processes send to eachother via pipes in
 * order to communicate.
 * TODO: Is this the best way? */
typedef struct
{
    int code; /* For passing __Code enums */
    union
    {
        Track track;
        int seconds;
        int count;
    } data;
} Comm;

void comm_connect(int *fd_read, int *fd_write);
void comm_send(int pipe, const Comm *command);
void comm_recv(int pipe, Comm *command);
void comm_to_string(const Comm *command, char *string);

#endif /* AT_COMMUNICATE_TA */
