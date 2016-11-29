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
#include "log.h"


#define USE_FFMPEG
#ifdef USE_FFMPEG
#include "ffmpeg.h"
#endif

pid_t fork_me( void (*go)(int *, int *), int *read, int *write )
{
  pid_t pid = fork();

  if (pid == 0)
    /* This process is the child process. */
    go(read, write);
    /* TODO: What should the child process return? */
  else
    /* This process is the parent process. */
    return pid;

  /* TODO: Add error checking if process cannot be forked */
  /* pid returned to parent would be -1 so check errno */
}

int main (int argc, char* argv[])
{
  /* Fork media player library process. */
  /* pid_t player_pid = fork_me(&player);*/

  /* init_ui(); */

  int read;
  int write;
  make_connection(&read, &write);


  /* Fork input module process. */
  /*pid_t input_pid = fork_me(&input, NULL, &write); */

  /* Fork output module process. */
  /*pid_t output_pid = fork_me(output_ptr);*/

  // TODO: Fork or not to fork?
  play_me("/home/acalder/music/Steven_Wilson/Hand._Cannot._Erase./10.Happy_Returns.flac");
  exit(0);

  char *command = NULL;
  while (1)
  {
    recv_command(read, &command);

    /* TODO: Terminate the while loop when a specific key is pressed. */

    log_write("play_while");
    log_write(command);

    free(command);
  }

}
