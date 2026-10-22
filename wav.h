#ifndef WAV_H
#define WAV_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint32_t sampleRate;
    uint16_t channels;
    uint16_t bitDepth;
    float *samples;

    size_t numFrames;

} WavData;


int wavLoad(WavData *out, const char *path);
void freeWav(WavData *w);

#endif