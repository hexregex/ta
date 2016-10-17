#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <unistd.h>
#include <sys/types.h>

#include "player.h"
#include "ui.h"
#include "input.h"
#include "output.h"
#include "communicate.h"


pid_t fork_me( void (*go)(int *, int *), int *read, int *write)
{
  pid_t pid = fork();

  if (pid == 0)
    (*go)(read, write);
  else
    return pid;
}

int main (int argc, char* argv[])
{
  /*


  pid_t player_pid = fork_me(&player);*/


  int read;
  int write;
  connect(&read, &write);

  init_ui();

  pid_t input_pid = fork_me(&input, NULL, &write);
  /*pid_t output_pid = fork_me(output_ptr);*/

  char *command = NULL;
  while (1)
  {
    recv_command(read, &command);
    log_write("play_while");
    log_write(command);

    free(command);
  }

}


