/*  This file (input.h) is part of the ta application.
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
#ifndef AT_INPUT_TA
#define AT_INPUT_TA

typedef enum
{
    NOP, /* No operation, no input, no key, do nothing */
    UP,
    DOWN,
    LEFT,
    RIGHT,
    ESC,
    SPACE,
    SEMICOLON,
    H,
    J,
    K,
    L,
    Q
} InCode;

void load_keymap ();

void in_process_go();

#endif /* AT_INPUT_TA */
