#ifndef AT_FFMPEG_TA
#define AT_FFMPEG_TA

void ff_init();
void ff_dest();

void ff_open(const char* in_filename);
void ff_play();
void ff_pause();

#endif /* AT_FFMPEG_TA */
