#ifndef DSP_H
#define DSP_H

#include <stddef.h>

float dspPeak(const float *x, size_t n);
float dspRms(const float *x, size_t n);
void dspNaiveDFT(const *x, size_t n, float *re, float *im);

#endif