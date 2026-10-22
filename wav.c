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
    int retCode = -1;
    uint8_t tag[4];
    uint32_t size;
    uint8_t id[4];

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
    

    retCode = 0;
    done:
        fclose(f);
        return retCode;
}

void wavFree(WavData *w) {

}    