#ifndef AT_COMMUNICATE_TA
#define AT_COMMUNICATE_TA

#include "player.h"

/* This is the data type that the processes send to eachother via pipes in
 * order to communicate.
 * TODO: Is this the best way? */
typedef struct
{
    int code; /* For passing __Code enums */
    union
    {
        Track track;
        int seconds;
        int count;
    } data;
} Comm;

void comm_connect(int *fd_read, int *fd_write);
void comm_send(int pipe, const Comm *command);
void comm_recv(int pipe, Comm *command);
void comm_to_string(const Comm *command, char *string);

#endif /* AT_COMMUNICATE_TA */
