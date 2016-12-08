#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dlfcn.h>
#include <string.h>

#include "input.h"
#include "communicate.h"
#include "log.h"

struct opmap {
    int keypress;
    char code[5];
};

void load_keymap () {
    /* char * line[1024]; */
    FILE *key_map_stream = fopen("key_file", "r");
    int next_char;
    int i;
    for (i = 0; i < 1024; i++)

        next_char = fgetc(key_map_stream);

    {
    }
    while (next_char != '\0');

    /*opmap ops[6];*/

    fclose(key_map_stream);
}


void in_process_go(int fd_write_to_main) {
    void *dynahand = NULL;
    void (*in_init)();
    char (*in_keypress)();
    void (*in_dest)();

    /* The following section (until while) dynamically loads the
       libtermios_input library and links the functions needed.
       (Dynamic loading and linking at runtime: implementing a plug-in.) */

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


    while (1) {
        printf("input is here\n");
        log_write("input_while-start");

        /* Wait for input then send input code to main. */
        Comm in_comm;
        in_comm.code = in_keypress();
        log_write_comm(&in_comm);
        comm_send(fd_write_to_main, &in_comm);

        log_write("input_while-end");
    }

    *(void **)(&in_dest) = dlsym(dynahand, "tios_clean");
    if (in_dest == NULL)
        log_write(dlerror());
    else
        in_dest();

    if(dlclose(dynahand) != 0)
        log_write(dlerror());
}
