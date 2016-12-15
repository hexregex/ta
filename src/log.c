#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <time.h>

#include "communicate.h"

#define MAX_BUFF_SIZE 128
#define MAX_MESS_LEN (MAX_BUFF_SIZE - 1)

static pthread_mutex_t truncate_mutex;
static pthread_mutex_t atomic_log_mutex;
static int truncate = 1;
static int initialized = 0;
static FILE *log2;

static void log_init()
{
    initialized = 1;
    log2 = fopen("another", "w");

    fflush(log2);
    pthread_mutex_init(&truncate_mutex, NULL);
    pthread_mutex_init(&atomic_log_mutex, NULL);
}


/* Write a string to the log file. */
static
int write_log(char *string)
{
    FILE *log;

    pthread_mutex_lock(&truncate_mutex);
    if (truncate == 1)
    {
        log = fopen("logfile", "w");
        fclose(log);
        truncate = 0;
    }
    pthread_mutex_unlock(&truncate_mutex);

    log = fopen("logfile", "a");
    int count = fprintf(log, "%s\n", (char *)string);
    /* fprintf(stderr, "%s\n", log_message); */
    fclose(log);

    free(string);

    return count;
}

static
void log_pthread(void *(*go)(void *), void *data)
{
    pthread_t log_thread_id;
    pthread_create(&log_thread_id, NULL, go, (void *)data);
}

static struct timespec ts;
static inline void log_time(char *time_str)
{
    clock_gettime(CLOCK_REALTIME, &ts);
    sprintf(time_str, "%lu:%09i", (unsigned long)ts.tv_sec, ts.tv_nsec);
}

static inline void log_fprintf(FILE *stream, char *time_str, char *message, pthread_t thread_id, pid_t p_id)
{
    fprintf(stream, "%s %s %i%i %i %i\n", time_str, message, thread_id, p_id, thread_id, p_id);
}


static void *lw_go(void* message)
{
    char time_str[40];

    log_time(time_str);

    log_fprintf(log2, time_str, "lw_go before locking:", pthread_self(), getpid());
    fflush(log2);

    pthread_mutex_lock(&atomic_log_mutex);

    log_time(time_str);
    log_fprintf(log2, time_str, ">>lw_go between locking<<:", pthread_self(), getpid());
    write_log(message);

    pthread_mutex_unlock(&atomic_log_mutex);

    log_time(time_str);
    log_fprintf(log2, time_str, "<<< lw_go after locking >>>:", pthread_self(), getpid());

    return NULL;
}

void log_write(const char *message)
{
    if (!initialized) {
        log_init();
    }
    char *buffer = malloc(MAX_BUFF_SIZE);
    buffer[MAX_MESS_LEN] = '\0';
    strncpy(buffer, message, MAX_MESS_LEN);
    log_pthread(lw_go, buffer);
}

/*
int logf(const char *format, ...)
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
*/

typedef struct {
    char message[MAX_BUFF_SIZE];
    int value;
} MessageInt;

static void *lwi_go(void *data)
{
    pthread_mutex_lock(&atomic_log_mutex);

    char *buffer1 = malloc(MAX_BUFF_SIZE);
    strcpy(buffer1, ((MessageInt *)data)->message);
    char buffer2[20];
    sprintf(buffer2, ": %i", ((MessageInt *)data)->value);
    strcat(buffer1, buffer2);

    write_log(buffer1);

    free(data);

    pthread_mutex_unlock(&atomic_log_mutex);

    return NULL;

}

/* Convert an int to a string and write to the log file. */
void log_write_int(const char *message, int value)
{
    /*
    MessageInt *data = malloc(sizeof(MessageInt));
    data->message[MAX_MESS_LEN] = '\0';
    strncpy((char *)&data->message, message, MAX_MESS_LEN);
    data->value = value;

    log_pthread(lwi_go, data);
    */
}


static void *lwc_go(void *command)
{
    pthread_mutex_lock(&atomic_log_mutex);

    char *buffer = malloc(MAX_BUFF_SIZE);
    comm_to_string((const Comm *)command, buffer);

    write_log(buffer);

    pthread_mutex_unlock(&atomic_log_mutex);

    return NULL;
}

void log_write_comm(const Comm *command)
{
    /*
    log_pthread(lwc_go, (void *)command);
    */
}


void log_write_array_of_strings(char **arr_of_str)
{
    if (arr_of_str == NULL)
    {
        log_write("ERROR: log_write_array_of_strings received NULL");
        return;
    }
    if (arr_of_str[0] == NULL)
    {
        log_write("ERROR: log_write_array_of_strings received NULL first string.");
        return;
    }

    const int GROW_SIZE = 128;
    /* -1 because string length does not include trailing '\0' */
    int out_max_len = GROW_SIZE - 1;
    char *out = malloc(GROW_SIZE);
    out[0] = '\0';
    int out_len = 0;
    while (1)
    {   /* +1 for trailing '\n' to be added. */
        out_len += strlen(*arr_of_str) + 1;
        if (out_len > out_max_len)
        {
            do /* Grow out_max_len until it fits out_len. */
                out_max_len += GROW_SIZE;
            while (out_len > out_max_len);
            /* +1 to include trailing '\0'. */
            out = realloc(out, out_max_len + 1);
        }
        strcat(out, *arr_of_str);

        if (*(++arr_of_str) == NULL)
            break;
        /* If this is the last string don't add '\n'. */
        strcat(out, "\n");
    }
    log_pthread(lw_go, out);
}
