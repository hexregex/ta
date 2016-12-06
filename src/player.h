#ifndef AT_PLAYER_TA
#define AT_PLAYER_TA

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
