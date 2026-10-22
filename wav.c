#include "wav.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHUNK_SIZE (1u << 30)

static int read_u16(FILE *f, uint16_t *out) {
    uint8_t bytes[2];
    if (fread(bytes, 1, 2, f) != 2) return -1;
    *out = (uint32_t)bytes[0]
     | ((uint32_t)bytes[1] << 8);

    return 0;
}

static int read_u32(FILE *f, uint32_t *out) {
    uint8_t bytes[4];
    if (fread(bytes, 1, 4, f) != 4) return -1;
    *out = (uint32_t)bytes[0]
     | ((uint32_t)bytes[1] << 8)
     | ((uint32_t)bytes[2] << 16)
     | ((uint32_t)bytes[3] << 24);

    return 0;
}

int wavLoad(WavData *out, const char *path) {
    memset(out, 0, sizeof *out);

    int retCode = -1;
    uint8_t tag[4];
    uint32_t size;
    uint8_t id[4];

    uint32_t fmtSize;
    uint16_t format;

    //throw away variables
    uint32_t byteRate;
    uint16_t blockAlign;

    uint8_t dataid[4];
    uint32_t dataSize;

    int16_t *raw = NULL;

    FILE *f = fopen(path, "rb");
    if(!f){
        return -1;
    }

    if (fread(tag, 1, 4, f) != 4) goto done;
    if(memcmp(tag, "RIFF", 4) != 0){
        goto done;
    }

    if (read_u32(f, &size) != 0) goto done;

    if (fread(tag, 1, 4, f) != 4) goto done;
    if(memcmp(tag, "WAVE", 4) != 0){
        goto done;
    }

    if (fread(id, 1, 4, f) != 4) goto done;
    printf("%.4s\n", id); 

    if (read_u32(f, &fmtSize) != 0) goto done;
    if(fmtSize > MAX_CHUNK_SIZE){
        printf("exceeded max chunk size :( \n");
        goto done;
    }

    if (read_u16(f, &format) != 0) goto done;
    printf("format: %u\n", format);
    if(format != 1){
        printf("file is not PCM \n");
        goto done;
    }

    if (read_u16(f, &out->channels) != 0) goto done;
    if(channels == 0){
        goto done;
    }
    if (read_u32(f, &out->sampleRate) != 0) goto done;

    if (read_u32(f, &byteRate) != 0) goto done;
    if (read_u16(f, &blockAlign) != 0) goto done;

    if (read_u16(f, &out->bitDepth) != 0) goto done;
    if(out->bitDepth != 16){
        printf("bit-depth must be 16-bits!");
        goto done;
    }

    if (fmtSize != 16) {
        fseek(f, fmtSize - 16, SEEK_CUR);
    }

    if (fread(dataid, 1, 4, f) != 4) goto done;
    if(dataid != "data"){
        goto done;
    }
    printf("%.4s\n", dataid); 
    if (read_u32(f, &dataSize) != 0) goto done;
    if (dataSize > MAX_CHUNK_SIZE) goto done;
    printf("data size: %u\n", dataSize);
    

    size_t numSamples = dataSize / (out->bitDepth / 8);
    out->samples = malloc(numSamples * sizeof *out->samples);
    if (!out->samples) goto done;

    out->numFrames = numSamples / out->channels;

    raw = malloc(numSamples * sizeof *raw);
    if (!raw) goto done;
    if (fread(raw, sizeof *raw, numSamples, f) != numSamples) {
        goto done;
    }

    for(size_t i = 0; i < numSamples; i++){
        out->samples[i] = raw[i] / 32768.0f;
    }


    double seconds = (double)out->numFrames / out->sampleRate;
    printf("duration: %f seconds\n", seconds);


    retCode = 0;
    done:
        if (retCode != 0) { free(out->samples); out->samples = NULL; }
        fclose(f);
        free(raw);
        return retCode;
}

void wavFree(WavData *w) {
    free(w->samples);
    w->samples = NULL;
}    