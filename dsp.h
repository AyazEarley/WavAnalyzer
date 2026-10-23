#ifndef DSP_H
#define DSP_H

#include <stddef.h>

float dspPeak(const float *x, size_t n);
float dspRms(const float *x, size_t n);
size_t dspZeroCrosses(const float *n, size_t N);
void dspNaiveDFT(const *x, size_t n, float *re, float *im);
void dspRecursiveFFT(const float *in, size_t N, float *re, float *im);

#endif