#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

#include "input.h"

static struct termios tios;
static struct termios other;

/* Not a part of POSIX so implementing here.
 * Code straight from the Linux man page: termios(3). */
static inline void cfmakeraw(struct termios *termios_p)
{
           termios_p->c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP
                                   | INLCR | IGNCR | ICRNL | IXON);
           termios_p->c_oflag &= ~OPOST;
           termios_p->c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
           termios_p->c_cflag &= ~(CSIZE | PARENB);
           termios_p->c_cflag |= CS8;
}

void tios_init()
{

    tcgetattr(STDIN_FILENO, &other);
    tios = other;
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
