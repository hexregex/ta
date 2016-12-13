#ifndef AT_FFMPEG_TA
#define AT_FFMPEG_TA

void ff_init();
void ff_dest();

void ff_open(const char* in_filename);
void ff_play();
void ff_pause();
void ff_next();
void ff_previous();
void ff_repeat();
void ff_seek();

int ff_track_duration();

#endif /* AT_FFMPEG_TA */
