#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <string.h>

#include "log.h"
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
    char string[8];
    memset(string, 0, 8);
    int old_file_flags = fcntl(STDIN_FILENO, F_GETFL);
    /* Wait to read one character from stdin. */
    read(STDIN_FILENO, string, 1);
    /* Set file status flags on stdin so read() won't wait for input. */
    int new_file_flags = old_file_flags | O_NONBLOCK;
    fcntl(STDIN_FILENO, F_SETFL, new_file_flags);
    /* Read stdin if there is input else continue. */
    read(STDIN_FILENO, string + 1, 3);
    /* Reset file flags for stdin. */
    fcntl(STDIN_FILENO, F_SETFL, old_file_flags);

    log_write(string);

    return in_str_to_code(string);
}

void tios_clean()
{
    /* Restore terminal too original state. */
    tcsetattr(STDIN_FILENO, TCSANOW, &other);
}
