#ifndef AT_PLAYER_TA
#define AT_PLAYER_TA

#include <signal.h>
#include <stdint.h>

#define MAX_FILES 64

/* Global value for the current play time in seconds*/
long plr_sec_play_time;

/* Global value for (much more) precise play time in float */
double plr_play_time;

typedef enum
{
    PLAY,
    PAUSE,
    NEXT,
    PREVIOUS,
    REPEAT,
    SEEK,
    RESET
} PlrCode;

typedef struct
{
    uint16_t number;
    char name[264];
    char codec[32];
    uint32_t sample_rate; /* hertz */
    uint16_t duration; /* seconds */
} Track;

typedef struct
{
    /* There used to be other data here.  Leaving as is in case I
     * decide to add more again someday */
    pid_t out_pid;
    const char *file_names[MAX_FILES];
} PlrThreadData;

/* 'plr' == player */
void (*plr_init)();
void (*plr_open)(const char *);
void (*plr_play)();
void (*plr_pause)();
void (*plr_next)();
void (*plr_previous)();
void (*plr_repeat)();
void (*plr_seek)(int);
int (*plr_track_duration)();

void plr_set_track(Track *track,
                   int track_number,
                   const char *track_name,
                   int track_duration);

void *plr_thread_go(void *thread_arg);

#endif /* AT_PLAYER_TA */
