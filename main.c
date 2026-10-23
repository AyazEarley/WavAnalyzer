#include "wav.h"
#include "dsp.h"

int main(void) {
    WavData w = {0};
    int rc =wavLoad(&w, "../../test.wav");
    printf("load result: %d, frames: %zu, channels: %u\n", rc, w.numFrames, w.channels);
    float peak = dspPeak(w.samples, w.numFrames * w.channels);
    printf("peak: %f\n", peak);

    float rms = dspRms(w.samples, w.numFrames * w.channels);
    printf("RMS: %f\n", rms);

    wavFree(&w);
    return 0;
}