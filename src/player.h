#ifndef AT_PLAYER_TA
#define AT_PLAYER_TA

#define MAX_FILES 32
#include <pthread.h>

double plr_play_time;
long plr_sec_play_time;

typedef enum
{
    PLAY,
    PAUSE,
    NEXT,
    PREVIOUS,
    REPEAT,
    SEEK,
    RESET,
    TRACK
} PlrCode;

typedef struct
{
    /* There used to be other data here.  Leaving as is in case I
     * decide to add more again someday */
    pthread_t ta_thread_id;
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
void (*plr_seek)();

void *plr_thread_go(void *thread_arg);

#endif /* AT_PLAYER_TA */
