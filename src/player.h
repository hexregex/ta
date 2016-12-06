#ifndef AT_PLAYER_TA
#define AT_PLAYER_TA

/* 'plr' == player */
void (*plr_init)();
void (*plr_open)(const char *);
void (*plr_play)();
void (*plr_pause)();

void *plr_thread_go(void *thread_arg);

#endif /* AT_PLAYER_TA */
