#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <unistd.h>
#include <termios.h>

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

    struct termios tios;
    struct termios other;

    /* */
    tcgetattr(STDIN_FILENO, &other);
    tios = other;
    /*tios.c_lflag &= ~(ICANON | ECHO);*/
    cfmakeraw(&tios);
    tcsetattr(STDIN_FILENO, TCSANOW, &tios);

    while (1) {

      /* TODO: Figure out how to gracefully terminate this process
         when SIGTERM in recieved. */

      /*key = getch();*/

      key = getchar();

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


    /* Restore terminal too original state. */
    tcsetattr(STDIN_FILENO, TCSANOW, &other);
}
