#ifndef AT_OUTPUT_TA
#define AT_OUTPUT_TA

typedef enum
{
    PLAYING,
    PAUSED,
    TRACK,
    LOAD_TRACK_LIST,
    PLAY_TIME
} OutCode;

void out_process_go();

#endif /* AT_OUTPUT_TA */
