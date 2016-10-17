#include <stdio.h>
#include <curses.h>
#include "communicate.h"

void load_keymap () {
}

void input (int *read, int *write) {
    read = NULL;
    char key;


    while (1) {

      /* TODO: Figure out how to gracefully terminate this process
         when SIGTERM in recieved. */

      key = getch();

      log_write("input");
      log_write_int(key);

      char command[3];
      command[0] = key;
      command[1] = '\n';
      command[2] = '\0';

      send_command(*write, command);

      log_write("hello!");
    }
}
