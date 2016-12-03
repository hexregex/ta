#ifndef AT_COMMUNICATE_TA
#define AT_COMMUNICATE_TA

/*
play
pause
seek
next
previous
*/

/* Use union/struct instead of built in type so I can changelater */
typedef union {
    char code;
    int data;
} Comm;


void comm_connect(int *read, int *write);

void comm_send(int pipe, const Comm *command);
void comm_recv(int pipe, Comm *command);
void comm_to_string(const Comm *command, char *string);

#endif /* AT_COMMUNICATE_TA */
