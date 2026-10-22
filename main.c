#include "wav.h"

int main(void) {
    WavData w = {0};
    wavLoad(&w, "../../test.wav");
    wavFree(&w);
    return 0;
}