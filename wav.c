#include "wav.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHUNK_SIZE (1u << 30)

static int read_u16(FILE *f, uint16_t *out) {
}

int wavLoad(WavData *out, const char *path) {
    int retCode = -1;
    uint8_t tag[4];
    uint32_t size;
    uint8_t sizeBytes[4];
    uint8_t id[4];

    FILE *f = fopen(path, "rb");
    if(!f){
        return -1;
    }

    if (fread(tag, 1, 4, f) != 4) goto done;
    if(memcmp(tag, "RIFF", 4) != 0){
        goto done;
    }


    if (fread(sizeBytes, 1, 4, f) != 4) goto done;
    
    size = (uint32_t)sizeBytes[0]
     | ((uint32_t)sizeBytes[1] << 8)
     | ((uint32_t)sizeBytes[2] << 16)
     | ((uint32_t)sizeBytes[3] << 24);

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