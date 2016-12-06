#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

#include "input.h"

static struct termios tios;
static struct termios other;

void tios_init()
{

    tcgetattr(STDIN_FILENO, &other);
    tios = other;
    /* tios.c_lflag &= ~(ICANON | ECHO); */
    cfmakeraw(&tios);
    tcsetattr(STDIN_FILENO, TCSANOW, &tios);
}

InCode tios_keypress()
{
    return in_char_to_code( (unsigned char)getchar() );
}

void tios_clean()
{
    /* Restore terminal too original state. */
    tcsetattr(STDIN_FILENO, TCSANOW, &other);
}
