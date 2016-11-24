#include <stdio.h>

#include <libavutil/error.h>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <ao/ao.h>
#include <pulse/simple.h>

#include "sdl.h"

#define ERR_BUFF_SIZE 80
#define AVCODEC_MAX_AUDIO_FRAME_SIZE 192000


static inline void error_woe(int error)
{
  if (error != 0)
  {
    char err_str[ERR_BUFF_SIZE];

    /* Get back error string from FFmpeg */
    av_strerror(error, err_str, ERR_BUFF_SIZE);

    fprintf(stderr, "FFmpeg Error: %s\n", err_str);
    exit(1);
  }
}

static inline
int locate_audio_stream(AVFormatContext *format_context)
{
  /* Locate the audio stream */
  int stream = -1, i;
  for (i = 0; i < format_context->nb_streams; i++)
  {
    /* TODO: Compiler warns "codec" is deprecated. Fix. */
    if (format_context->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO)
    {
      stream = i;
      break;
    }
    // comments
  }

  /* TODO: Add better error handling. */
  if (stream == -1) exit(5);

  return stream;
}

void play_me(const char *const filename)
{
  AVFormatContext *format_context = NULL;
  AVCodecContext *codec_contextSave = NULL;
  AVCodecContext *codec_context = NULL;
  AVCodec *codec = NULL;
  AVDictionary *dictionary;
  AVPacket *packet;
  AVFrame *av_frame;


  /* Register all the codecs. */
  av_register_all();

  /* Open the media file. */
  error_woe( avformat_open_input(&format_context, filename, NULL, NULL) );

  /* Get information about the media stream. */
  error_woe( avformat_find_stream_info(format_context, NULL) );

  /* Output stream info to stderr */
  av_dump_format(format_context, 0, filename, 0);

  int stream = ta_get_audio_stream(format_context);
                                 _
  /* fprintf(stderr, "%i", stream); */



  /* codec_context = format_context->streams[stream]->codec; */
  codec_context = ta_get_codec_context(AVFormatContext);

  codec = avcodec_find_decoder(codec_context->codec_id);

  if (codec == NULL)
  {
    /* TODO: Add better error handling. */
    fprintf(stderr, "FFmpeg Error: Codec is not supported.\n");
    exit(4);
  }


  codec_context = avcodec_alloc_context3(codec);

  /* Open the codec */
  error_woe( avcodec_open2(codec_context, codec, NULL) );

    av_init_packet(packet);

    /* Allocate the frame */
    av_frame = av_frame_alloc();

    int buffer_size = AVCODEC_MAX_AUDIO_FRAME_SIZE;
    uint8_t buffer[buffer_size];
    packet->data = buffer;
    packet->size = buffer_size;


    int len;
    int frameFinished =0;
    while( avcodec_receive_frame(codec_context, av_frame) >= 0)
    {
        fprintf(stderr, "made 3\n");

        // Decodes from `packet` into the buffer
        /*
        if (avcodec_decode_audio4(codec_context, (int16_t*)buffer, &buffer_size, &packet) < 1) {
            break;  // Error in decoding
        }
        */

        int error;
        error_woe( error = avcodec_send_packet(codec_context, packet) );
        fprintf(stderr, "made 4\n");

        error_woe( avcodec_receive_frame(codec_context, av_frame) );
        fprintf(stderr, "made 5\n");

        // Send the buffer contents to the audio device
        /*ao_play(device, av_frame-(char*)buffer, buffer_size); */



    }
    av_packet_free(packet);
    av_frame_free(&av_frame);

    avformat_close_input(&format_context);


  fprintf(stderr, "\nYou made it!\n");
}


/* code to initialize pulse audio */
/*
pa_simple *s;
pa_sample_spec ss;
ss.format = PA_SAMPLE_S16NE;
ss.channels = 2;
ss.rate = 44100;

s = pa_simple_new(NULL,               // Use the default server.
                "Fooapp",           // Our application's name.
                PA_STREAM_PLAYBACK,
                NULL,               // Use the default device.
                "Music",            // Description of our stream.
                &ss,                // Our sample format.
                NULL,               // Use default channel map
                NULL,               // Use default buffering attributes.
                NULL               // Ignore error code.
                );

pa_simple_write (s, av_frame->data[0], AVCODEC_MAX_AUDIO_FRAME_SIZE, &error);
*/
