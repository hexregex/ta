#ifndef AT_COMMUNICATE_TA
#define AT_COMMUNICATE_TA


typedef enum
{
    PLAY,
    PAUSE,
    SEEK,
    RESTART,
    RESET,
    NEXT,
    PREVIOUS,
    TRACK
} Code;


/* Use union/struct instead of built in type so I can change later. */
/* TODO: Is this the best way? */
typedef struct
{
    Code code;
    union
    {
        int i;
        unsigned char c;
        char *s;
        char **as; /* 'as' == array of strings */
    } data;
} Comm;


void comm_connect(int *fd_read, int *fd_write);

void comm_send(int pipe, const Comm *command);
void comm_recv(int pipe, Comm *command);

void comm_to_string(const Comm *command, char *string);

#endif /* AT_COMMUNICATE_TA */
