
void connect (int *read, int *write);

void send_command(int pipe, char *command);
void recv_command(int pipe, char **command);

int log_write(char *log_message);

int log_write_int(int value);

