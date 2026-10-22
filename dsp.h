#ifndef DSP_H
#define DSP_H

#include <stddef.h>
#include <math.h>

float dspPeak(const float *x, size_t n);
float dspRms(const float *x, size_t n);

#endif