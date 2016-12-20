/*  This file (ta.h) is part of the ta application.
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
#ifndef AT_TA_TA
#define AT_TA_TA

/* File descriptors for the communication pipes between modules.
 * The numbers indicate the probable values assigned.
 * 'ta' == main, 'in' == input, 'out' == output, 'plr' == player
.__________________________________________________,
|*/  int ta_read_from_in;   /*  4| read from input |\
|*/  int in_write_to_ta;    /*  5|_________________| \
|*/  int out_read_from_ta;  /*  6| write to output |\ |
|*/  int ta_write_to_out;   /*  7| _ _ _ _ _ _ _ _ | \|
|*/  int out_read_from_plr; /*  8|  (from player)  |. |
|*/  int plr_write_to_out;  /*  9|_________________| .|
|*/  int plr_read_from_ta;  /* 10| write to player |\ |
|*/  int ta_write_to_plr;   /* 11|                 | \|
|________________________________|_________________|  |
 \                                \                 \ |
  \________________________________\_________________\|

       */
#endif /* AT_TA_TA
     */
