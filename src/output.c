#include <stdio.h>
#include <fcntl.h>

#include "ta.h"
#include "log.h"
#include "output.h"
#include "communicate.h"

void out_process_go()
{
    // int file_flags = fcntl(fd_read, F_GETFL);
    // printf("output flag non-block = %i\n", file_flags);
    while (1)
    {
        log_write("output_while-start");
        Comm command;
        comm_recv(out_read_from_ta, &command);

        log_write_comm(&command);
        log_write("output_while-end");
    }
}
