#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

#include "log.h"

static struct termios tios;
static struct termios other;

void input_init()
{

    tcgetattr(STDIN_FILENO, &other);
    tios = other;
    /* tios.c_lflag &= ~(ICANON | ECHO); */
    cfmakeraw(&tios);
    tcsetattr(STDIN_FILENO, TCSANOW, &tios);
}

char input_keypress()
{
    return getchar();
}

void input_clean()
{
    /* Restore terminal too original state. */
    tcsetattr(STDIN_FILENO, TCSANOW, &other);
}
