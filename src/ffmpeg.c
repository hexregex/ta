#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <signal.h>

#include <libavutil/samplefmt.h>
#include <libavutil/mathematics.h>
#include <libavutil/error.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <ao/ao.h>
#include <pulse/simple.h>
#include <string.h>

#include "log.h"
#include "player.h"

#define ERR_BUFF_SIZE 80
#define AVCODEC_MAX_AUDIO_FRAME_SIZE 192000
#define BUFFER_SIZE AVCODEC_MAX_AUDIO_FRAME_SIZE + FF_INPUT_BUFFER_PADDING_SIZE

static void error_woe(const char *message)
{
    char *buffer;
    buffer = (char *)malloc(strlen(message) + 15);
    sprintf(buffer, "FFmpeg Error: %s", message);
    log_write(buffer);
    free((void *)buffer);
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
int ff_get_audio_stream(AVFormatContext *format_context)
{
    /* Locate the audio stream */
    int stream = -1;
    unsigned int i;
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


static AVFormatContext *container = NULL;
static int stream_id;
static AVCodecContext *ctx = NULL;

static ao_sample_format sformat;
static ao_device *adevice = NULL;
static AVPacket p;
static AVPacket *packet = &p; /* Never use variable p again */


void ff_init()
{
    /* Register all the codecs. */
    av_register_all();

    /* Initialize the AO library */
    ao_initialize();
}

void ff_dest()
{
    if (container != NULL)
        avformat_close_input(&container);

    ao_shutdown();
}


/* Load an audio file. */
void ff_open(const char *in_filename) {

    if (container != NULL)
        avformat_close_input(&container);

    container = avformat_alloc_context();

    /* Open the media file. */
    ff_error_woe( avformat_open_input(&container, in_filename, NULL, NULL) );

    /* Get information about the media stream. */
    ff_error_woe( avformat_find_stream_info(container, NULL) );

    /* Output stream info to stderr */
    av_dump_format(container,0,in_filename,0);

    stream_id = ff_get_audio_stream(container);

    /* TODO: Is this used by anything? */
    /* AVDictionary *metadata = container->metadata; */

    ctx = container->streams[stream_id]->codec;
    AVCodec *codec = avcodec_find_decoder( ctx->codec_id );
    if (codec == NULL) error_woe("FFmpeg Error: Codec is not supported.");

    /* Open the codec */
    if ( avcodec_open2(ctx, codec, NULL) < 0 )
        error_woe("Codec cannot be found.");

    /* TODO: Is this required? */
    /* ctx=avcodec_alloc_context3(codec); */

    /* TODO: Switched to get rid of warnings.  See if I can get to work
     * with enumerated type. */
    /* enum AVSampleFormat sfmt = ctx->sample_fmt; */
    int sfmt = ctx->sample_fmt;
    switch (sfmt)
    {
        case AV_SAMPLE_FMT_U8:
            log_write("U8");
            sformat.bits = 8;
            break;
        case AV_SAMPLE_FMT_S16:
            log_write("S16");
            sformat.bits = 16;
            break;
        case AV_SAMPLE_FMT_S32:
            log_write("S32");
            sformat.bits = 32;
            break;
    }

    sformat.channels = ctx->channels;
    sformat.rate = ctx->sample_rate;
    sformat.byte_format = AO_FMT_NATIVE;
    sformat.matrix = "L,R";
    /* sformat.matrix = 0; */

    int driver = ao_default_driver_id();
    adevice = ao_open_live(driver, &sformat, NULL);
    int error = errno;
    if (adevice == NULL)
    {
        log_write_int("libao error number", error);
    }

}

void ff_play()
{
    av_init_packet(packet);

    /* Allocate the frame */
    AVFrame *frame = av_frame_alloc();

    uint8_t buffer[BUFFER_SIZE];
    packet->data = buffer;

    /* TODO: Add error detection. */
    /* if (container == NULL) return -5; */
    int frameFinished = 0;
    while ( av_read_frame(container, packet) >= 0 )
    {
        if (packet->stream_index == stream_id)
        {
            /* TODO: Add error detection. */
            /* if (ctx == NULL) return -4; */
            /* int len = */
            avcodec_decode_audio4(ctx, frame, &frameFinished, packet);

            /* TODO seems like a waste of processing time to convert a
               rational number to a double every pass through the loop.
               See if I can optimize somehow. */
            plr_play_time =
                av_q2d( av_mul_q(container->streams[stream_id]->time_base,
                                 (AVRational){ packet->pts, 1 }) );

            if (frameFinished)
            {
                ao_play(adevice, (char*)frame->extended_data[0],frame->linesize[0] );
            } else {
                //log_write("Not finished");
            }
        }
    }

    av_packet_unref(packet);
    av_frame_free(&frame);
}

void ff_pause()
{
}


void ff_next()
{
}

void ff_previous()
{
}

void ff_repeat()
{
}

void ff_seek(int seconds)
{

    /* TODO:  Occasionally this operation causes a segfault.  Probably
     * something below (likely in av_seek_frame) which changes a value,
     * reallocates memory, or changes a pointer.  When the signal
     * handler which called this function exits a segfault is caused when
     * execution continues in ff_play where the signal was caught.
     * Find a solution.  Add mutex? Semaphore? Tell ff_play to restart
     * it's play loop if a signal was caught? */
    log_write_int("ff_seek--start", seconds);

    int64_t seek_pos =
        seconds
        * (int64_t)av_q2d(av_div_q( (AVRational){1, 1},
                                    container->streams[stream_id]->time_base ))
        + packet->pts;

    av_seek_frame( container,
                   stream_id,
                   seek_pos,
                   (seconds < 0) ? AVSEEK_FLAG_BACKWARD : 0 );

}


