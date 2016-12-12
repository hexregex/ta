#ifndef AT_OUTPUT_TA
#define AT_OUTPUT_TA

/* TODO: ISO C defines these somewhere. */
#define true 1
#define false 0

typedef enum
{
    PLAYING,
    PAUSED,
    TRACK,
    LOAD_TRACK_LIST,
    LOAD_TRACK,
    PLAY_TIME,
    TRACK_LIST
} OutCode;

void seconds_to_str(char *str, int val);

void out_process_go();

#endif /* AT_OUTPUT_TA */
