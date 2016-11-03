#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dlfcn.h>

#include "communicate.h"
#include "log.h"

/*
   play
   paus
   seek
   next
   prev
   quit
 */


struct opmap {
    int keypress;
    char code[5];
};



void load_keymap () {

  char * line[1024];
  FILE *key_map_stream = fopen("key_file", "r");
  int next_char;
  for (int i = 0; i < 1024; i++)

    next_char = fgetc(key_map_stream);

  {
  }
  while (next_char != '\0');



  /*opmap ops[6];*/

  fclose(key_map_stream);
}

void input (int *read, int *write) {
    read = NULL;
    char key;

    void *dynahand = NULL;
    void (*input_init)();
    char (*input_keypress)();
    void (*input_clean)();

    char *error;

    /* TODO make the selection of input library to load conditional. */
    dlerror();
    dynahand = dlopen("./libtermios_input.so", RTLD_LAZY);
    if (dynahand == NULL)
      log_write(dlerror());

    /* the following code segments maybe could be done with a function */
    *(void **)(&input_init) = dlsym(dynahand, "input_init");
    if (input_init == NULL)
      log_write(dlerror());
    else
      input_init();

    *(void **)(&input_keypress) = dlsym(dynahand, "input_keypress");
    if (input_keypress == NULL)
      log_write(dlerror());


    while (1) {

      /* TODO: Figure out how to gracefully terminate this process
         when SIGTERM in recieved. */

      /*key = getch();*/

      key = input_keypress();

      char command[3];
      command[0] = key;
      command[1] = '\n';
      command[2] = '\0';

      log_write("input");
      log_write_int(key);
      log_write(command);

      send_command(*write, command);

      log_write("hello!");
    }

    *(void **)(&input_clean) = dlsym(dynahand, "input_clean");
    if (input_clean == NULL)
      log_write(dlerror());
    else
      input_clean();

    if(dlclose(dynahand) != 0)
      log_write(dlerror());
}
