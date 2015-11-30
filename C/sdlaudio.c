#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#define USE_SDL2    1

#define AUDIO_SIZE  (1 << 20)
#define AUDIO_DTYPE Uint16
#define FREQ 44100
#define MIN_AMP -USHRT_MAX
#define MAX_AMP USHRT_MAX

static Uint8 *audio_chunk;
static Uint32 audio_len;
static Uint8 *audio_pos;


   /* The audio function callback takes the following parameters:
       stream:  A pointer to the audio buffer to be filled
       len:     The length (in bytes) of the audio buffer
    */
void fill_audio(void *udata, Uint8 *stream, int len)
{
        /* Only play if we have data left */
        if ( audio_len == 0 )
            return;

        /* Mix as much data as possible */
        len = ( len > audio_len ? audio_len : len );
        SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);
        audio_pos += len;
        audio_len -= len;
}

void pulse(unsigned int freq, AUDIO_DTYPE* data, size_t len)
{
    size_t i;
    unsigned int wavelen,halflen;

    wavelen = FREQ/freq;
    halflen = wavelen/2;
    for(i = 0; i < len; i += wavelen)
    {
        memset(data + i, MAX_AMP, sizeof(AUDIO_DTYPE)*halflen);
        memset(data + i + halflen, MIN_AMP, sizeof(AUDIO_DTYPE)*halflen);
    }
}

void triangle(unsigned int freq, AUDIO_DTYPE* data, size_t len)
{
    size_t i, j;
    unsigned int wavelen;
    float slope;

    wavelen = FREQ/freq;
    slope = (float)(MAX_AMP - MIN_AMP)/wavelen;
    for(i = 0; i < len; i += wavelen)
    {
        for(j = 0; j < wavelen; j ++)
            data[i + j] = (AUDIO_DTYPE)slope*j - 127;
    }
}

void random_audio(AUDIO_DTYPE* data, size_t len)
{
    FILE* fp;

    fp = fopen("/dev/urandom", "r");
    if(fp)
    {
        fread(data, sizeof(AUDIO_DTYPE), len, fp);
        fclose(fp);
    }
    else
    {
        size_t i;
        srand(time(0));
        for(i = 0; i < len; i ++)
            data[i] = rand();
    }
}

void play_audio()
{
    SDL_AudioSpec wanted;
    /* Set the audio format */
    wanted.freq = FREQ;
    //wanted.format = AUDIO_S16;
    wanted.format = AUDIO_S16;
    wanted.channels = 1;    /* 1 = mono, 2 = stereo */
    wanted.samples = 1024;  /* Good low-latency value for callback */
    wanted.callback = fill_audio;
    wanted.userdata = NULL;

    /* Open the audio device, forcing the desired format */
    if ( SDL_OpenAudio(&wanted, NULL) < 0 ) {
        fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
        exit(-1);
    }
    /* Load the audio data ... */
    audio_len = AUDIO_SIZE;
    audio_chunk = (Uint8*)malloc(AUDIO_SIZE*sizeof(AUDIO_DTYPE));
    audio_pos = audio_chunk;

    /* Let the callback function play the audio chunk */
    SDL_PauseAudio(0);

    /* Do some processing */
    {
        size_t i;
        unsigned int freq = 300;
        size_t interval = FREQ;
        int audio_type = 0;
        for(i = 0; i < (AUDIO_SIZE - interval); i += interval)
        {
            if(audio_type == 0)
            {
                triangle(freq, (AUDIO_DTYPE*)audio_chunk + i, interval);
                audio_type = 1;
            }
            else
            {
                pulse(freq, (AUDIO_DTYPE*)audio_chunk + i, interval);
                audio_type = 0;
            }
            freq += 50;
        }
    }

    ;;;;;

    /* Wait for sound to complete */
    while ( audio_len > 0 ) {
        SDL_Delay(100);         /* Sleep 1/10 second */
    }
    SDL_CloseAudio();
}

int main()
{
    int i, devcount;

    SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO);
    
    devcount = SDL_GetNumAudioDevices(0);
    printf("Number of audio devices: %d\n", devcount);
    for(i = 0; i < devcount; i ++)
        printf("Audio device %d: %s\n", i, SDL_GetAudioDeviceName(i, 0));
    
    SDL_Quit();

    return 0;
}
