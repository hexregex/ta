#include <stdio.h>
#include <curses.h>
#include "communicate.h"

void load_keymap () {
}

void input (int *read, int *write) {
    read = NULL;


    char key;


    while (1) {
      key = getch();

      char command[3] = "\0";
      command[0] = key;
      command[1] = '\n';

      send_command(*write, command);
    }
}
