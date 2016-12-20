/*  This file (input.c) is part of the ta application.
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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dlfcn.h>
#include <string.h>

#include "ta.h"
#include "input.h"
#include "communicate.h"
#include "log.h"

/* TODO: Finish this early work done which was never completed. */
/*
struct opmap {
    int keypress;
    char code[5];
};

void load_keymap () {
*/
    /* char * line[1024]; */
/*
    FILE *key_map_stream = fopen("key_file", "r");
    int next_char;
    int i;
    for (i = 0; i < 1024; i++)

        next_char = fgetc(key_map_stream);

    {
    }
    while (next_char != '\0');
*/
    /*opmap ops[6];*/
/*
    fclose(key_map_stream);
}
*/

void in_process_go()
{
    void *dynahand = NULL;

    void (*in_init)();
    char (*in_keypress)();
    void (*in_dest)();

    /* The following section (until while) dynamically loads the
     * libtermios_input library and links the functions needed.
     * (Dynamic loading and linking at runtime: implementing a plug-in.) */

    /* TODO make the selection of input library to load conditional. */
    dlerror();
    dynahand = dlopen("./libtermios_input.so", RTLD_LAZY);
    if (dynahand == NULL)
        log_write(dlerror());

    /* the following code segments maybe could be done with a function */
    *(void **)(&in_init) = dlsym(dynahand, "tios_init");
    if (in_init == NULL)
        log_write(dlerror());
    else
        in_init();

    *(void **)(&in_keypress) = dlsym(dynahand, "tios_keypress");
    if (in_keypress == NULL)
        log_write(dlerror());

    while (1)
    {
        log_write("input_while-start");

        /* Wait for input then send input code to main. */
        Comm in_comm;
        in_comm.code = in_keypress();
        log_write_comm(&in_comm);
        comm_send(in_write_to_ta, &in_comm);

        log_write("input_while-end");
    }

    *(void **)(&in_dest) = dlsym(dynahand, "tios_clean");
    if (in_dest == NULL)
        log_write(dlerror());
    else
        in_dest();

    if (dlclose(dynahand) != 0)
        log_write(dlerror());
}
