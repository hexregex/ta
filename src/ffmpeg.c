#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <libavutil/samplefmt.h>
#include <libavutil/mathematics.h>
#include <libavutil/error.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <ao/ao.h>
#include <pulse/simple.h>

#define ERR_BUFF_SIZE 80
#define AVCODEC_MAX_AUDIO_FRAME_SIZE 192000
#define BUFFER_SIZE AVCODEC_MAX_AUDIO_FRAME_SIZE + FF_INPUT_BUFFER_PADDING_SIZE

static void error_woe(const char *message)
{
  // hello
    fprintf(stderr, "FFmpeg Error: %s\n", message);
    exit(1);
}


static inline
void ff_error_woe(int error)
{
  if (error != 0)
  {
    char err_str[ERR_BUFF_SIZE];

    /* Get back error string from FFmpeg */
    av_strerror(error, err_str, ERR_BUFF_SIZE);

    error_woe(err_str);
  }
}

static inline
int ta_get_audio_stream(AVFormatContext *format_context)
{
  /* Locate the audio stream */
  int i, stream = -1;
  for (i = 0; i < format_context->nb_streams; i++)
  {
    /* TODO: Compiler warns "codec" is deprecated. Fix. */
    if ( format_context->streams[i]->codec->codec_type
         == AVMEDIA_TYPE_AUDIO )
    {
      stream = i;
      break;
    }
  }
  if (stream == -1) error_woe("Could not find Audio Stream");

  return stream;
}

void play_me(const char *const input_filename)
{
    /* Register all the codecs. */
    av_register_all();

    AVFormatContext *container = avformat_alloc_context();
    /* Open the media file. */
    ff_error_woe( avformat_open_input(&container, input_filename, NULL, NULL) );

    /* Get information about the media stream. */
    ff_error_woe( avformat_find_stream_info(container, NULL) );

    /* Output stream info to stderr */
    av_dump_format(container,0,input_filename,0);

    int stream_id = ta_get_audio_stream(container);

    /* TODO: Is this used by anything? */
    /* AVDictionary *metadata = container->metadata; */

    AVCodecContext *ctx = container->streams[stream_id]->codec;
    AVCodec *codec = avcodec_find_decoder( ctx->codec_id );
    if (codec == NULL) error_woe("FFmpeg Error: Codec is not supported.");

    /* Open the codec */
    if ( avcodec_open2(ctx, codec, NULL) < 0 )
        error_woe("Codec cannot be found.");

    /* TODO: Is this required? */
    /* ctx=avcodec_alloc_context3(codec); */

    /* Initialize the AO library */
    ao_initialize();

    ao_sample_format sformat;
    enum AVSampleFormat sfmt = ctx->sample_fmt;
    switch (sfmt)
    {
        case AV_SAMPLE_FMT_U8:
            printf("U8\n");
            sformat.bits = 8;
            break;
        case AV_SAMPLE_FMT_S16:
            printf("S16\n");
            sformat.bits = 16;
            break;
        case AV_SAMPLE_FMT_S32:
            printf("S32\n");
            sformat.bits = 32;
            break;
    }

    sformat.channels = ctx->channels;
    sformat.rate = ctx->sample_rate;
    sformat.byte_format = AO_FMT_NATIVE;
    /*sformat.matrix = "L,R";*/
    sformat.matrix = 0;

    int driver = ao_default_driver_id();
    ao_device *adevice = ao_open_live(driver, &sformat, NULL);
    int error = errno;
    if (adevice == NULL)
    {
      fprintf(stderr, "error number:%i\n", error);
    }

    AVPacket p, *packet = &p; /* never use variable p again */
    /* TODO is this better? */
    /* AVPacket packet = malloc( sizeof( AVPacket ) ); */
    av_init_packet(packet);

    /* Allocate the frame */
    AVFrame *frame = av_frame_alloc();

    uint8_t buffer[BUFFER_SIZE];
    packet->data = buffer;
    packet->size = BUFFER_SIZE;

    int frameFinished = 0;
    while ( av_read_frame(container, packet) >= 0 )
    {
        if (packet->stream_index == stream_id)
        {
            /* int len = avcodec_decode_audio4(ctx, frame, &frameFinished, packet); */
            avcodec_decode_audio4(ctx, frame, &frameFinished, packet);

            if (frameFinished)
            {
                ao_play(adevice, (char*)frame->extended_data[0],frame->linesize[0] );
            } else {
                //printf("Not Finished\n");
            }
        } else printf("Some other packet possibly video\n");
    }

    av_packet_unref(packet);
    av_frame_free(&frame);

    ao_shutdown();
    avformat_close_input(&container);

    fprintf(stderr, "\nYou made it!\n");
}
