#ifndef AT_COMMUNICATE_TA
#define AT_COMMUNICATE_TA

#include <stdint.h>

typedef struct
{
    uint16_t number;
    char name[264];
    char codec[32];
    uint32_t sample_rate; /* hertz */
    uint16_t duration; /* seconds */
} Track;

/* Use union/struct instead of built in type so I can change later. */
/* TODO: Is this the best way? */
typedef struct
{
    int code; /* For passing __Code enums */
    union
    {
        int i;
        unsigned char c;
        char *s;
        char **as; /* 'as' == array of strings */
        int count;
        int seconds;
        Track track;
    } data;
} Comm;


void comm_connect(int *fd_read, int *fd_write);

void comm_send(int pipe, const Comm *command);
void comm_recv(int pipe, Comm *command);

void comm_to_string(const Comm *command, char *string);

#endif /* AT_COMMUNICATE_TA */
