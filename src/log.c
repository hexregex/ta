/*  This file (log.c) is part of the ta application.
    Copyright (C) 2016-2017 Aaron Calder

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "communicate.h"

int truncate = 1;

/* Write a string to the log file. */
int log_write(const char *log_message)
{
  int count;
  FILE *log;
  if (truncate == 1)
  {
    log = fopen("logfile", "w");
    fclose(log);
    truncate = 0;
  }
  log = fopen("logfile", "a");
  count = fprintf(log, "%s\n", log_message);
  //count = fprintf(stderr, "%s\n", log_message);
  fclose(log);
  return count;
}

/* Convert an int to a string and write to the log file. */
int log_write_int(const char *log_message, int value)
{
    char buffer1[80];
    strcpy(buffer1, log_message);
    char buffer2[20];
    sprintf(buffer2, ": %i", value);
    strcat(buffer1, buffer2);
    return log_write(buffer1);
}

int log_write_comm(const Comm *command)
{

    char comm_string[80];
    comm_to_string(command, comm_string);
    return log_write(comm_string);
}

void log_write_array_of_strings(const char **abc)
{
    while (*abc != NULL) log_write(*abc++);
}
