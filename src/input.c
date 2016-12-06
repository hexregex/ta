#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dlfcn.h>

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
    void (*input_init)();
    char (*input_keypress)();
    void (*input_clean)();

    /* TODO make the selection of input library to load conditional. */
    dlerror();
    dynahand = dlopen("./libtermios_input.so", RTLD_LAZY);
    if (dynahand == NULL)
        log_write(dlerror());

    /* the following code segments maybe could be done with a function */
    *(void **)(&input_init) = dlsym(dynahand, "tios_init");
    if (input_init == NULL)
        log_write(dlerror());
    else
        input_init();

    *(void **)(&input_keypress) = dlsym(dynahand, "tios_keypress");
    if (input_keypress == NULL)
        log_write(dlerror());


    while (1) {
    printf("input is here\n");
    log_write("input_while-start");

        /* TODO: Figure out how to gracefully terminate this process
           when SIGTERM in recieved. */

        /*key = getch();*/

        Comm xyz = {input_keypress()};
        Comm *command = &xyz;

        log_write_comm(command);

        comm_send(fd_write_to_main, command);

        log_write("input_while-end");
    }

    *(void **)(&input_clean) = dlsym(dynahand, "tios_clean");
    if (input_clean == NULL)
        log_write(dlerror());
    else
        input_clean();

    if(dlclose(dynahand) != 0)
        log_write(dlerror());
}
