#include <stdio.h>

#include <libavutil/error.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>

#define ERR_BUFF_SIZE 80


static inline void error_woe(int error)
{
  if (error != 0)
  {
    char err_str[ERR_BUFF_SIZE];

    /* Get back error string from FFmpeg */
    av_strerror(error, err_str, ERR_BUFF_SIZE);

    fprintf(stderr, "FFmpeg Error: %s\n", err_str);
    exit(-1);
  }
}


void play_me(const char *const filename)
{

  AVFormatContext *fContext = NULL;

  /* Register all the codecs. */
  av_register_all();

  /* Open the media file. */
  error_woe( avformat_open_input(&fContext, filename, NULL, NULL) );

  /* Get information about the media stream. */
  error_woe( avformat_find_stream_info(fContext, NULL) );

  /* Output stream info to stderr */
  av_dump_format(fContext, 0, filename, 0);


}
