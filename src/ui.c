#include <curses.h>
#include "ui.h"


static WINDOW * screen;

void init_ui ()
{
  screen = initscr();
  noecho();
  cbreak();
}

void close_ui ()
{
  endwin();
}
