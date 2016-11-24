#include <libavutil/frame.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>

#include <ao/ao.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_thread.h>
#include <errno.h>
#include <pulse/simple.h>

#define AVCODEC_MAX_AUDIO_FRAME_SIZE 192000

static inline void error_woe(int error)
{
  if (error != 0)
  {
    char err_str[80];
    av_strerror(error, err_str, 80);
    fprintf(stderr, "What's this error?: %s\n", err_str);
    exit(1);
  }
}

  /* error_woe( SDL_Init(SDL_INIT_AUDIO) ); */

void sound_me(AVFormatContext *container, AVCodecContext *codec_context, AVCodec *codec)
{

  /* SDL_Init(SDL_INIT_AUDIO);
  exit(0); */

  /*ao_initialize(); */

  /*int driver = ao_default_driver_id(); */
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
                  */

  /* fprintf(stderr, "driver: %i\n", driver); */

/*
  ao_sample_format sample_format;
  sample_format.bits = 16;
  sample_format.channels = 2;
  sample_format.rate = 44100;
  sample_format.byte_format = AO_FMT_NATIVE;
  sample_format.matrix = 0;
  */

  /*fprintf(stderr, "made 1\n"); */

  /*
  ao_device* device;
  device = ao_open_live(driver, &sample_format, NULL);
  error_woe(errno);
  */


  /* fprintf(stderr, "made 2\n"); */

  /*

    AVPacket packet;
    av_init_packet(&packet);
    AVFrame *av_frame = av_frame_alloc();
    int buffer_size = AVCODEC_MAX_AUDIO_FRAME_SIZE;
    uint8_t buffer[buffer_size];
    packet.data = buffer;
    packet.size = buffer_size;

    while( avcodec_receive_frame(codec_context, av_frame) >= 0)
    while (av) {
        fprintf(stderr, "made 3\n");

        if (packet.stream_index == stre
        buffer_size = AVCODEC_MAX_AUDIO_FRAME_SIZE;

        // Decodes from `packet` into the buffer
        */
        /*
        if (avcodec_decode_audio4(codec_context, (int16_t*)buffer, &buffer_size, &packet) < 1) {
            break;  // Error in decoding
        }
        */

  /*
        int error;
        error_woe( error = avcodec_send_packet(codec_context, &packet) );
        fprintf(stderr, "made 4\n");

        error_woe( avcodec_receive_frame(codec_context, av_frame) )
        fprintf(stderr, "made 5\n");

        // Send the buffer contents to the audio device
        */
        /*ao_play(device, av_frame-(char*)buffer, buffer_size); */

          /*
        pa_simple_write (s, av_frame->data[0], AVCODEC_MAX_AUDIO_FRAME_SIZE, &error);
    }

    av_packet_free(&packet);
    av_frame_free(&av_frame);

    avformat_close_input(&container);

    ao_shutdown();

    fprintf(stdout, "Done playing. Exiting...");
    */
}

