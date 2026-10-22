#include "dsp.h"
#include <math.h>

#define PI 3.14159265358979323846

float dspPeak(const float *x, size_t n){
    float peak = 0;
    for(size_t i = 0; i < n; i++){
        float absSample = fabsf(x[i]);
        if(absSample > peak){
            peak = absSample;
        }
    }
    return peak;
}

float dspRms(const float *x, size_t n){
    if(n == 0) return 0.0f;
    double total = 0;
    for(size_t i = 0; i < n; i++){
        total = total + ((double)x[i] * x[i]);
    }
    total = total / n;
    total = sqrt(total);
    return (float)total;
}

void dspNaiveDFT(const float *x, size_t N, float *re, float *im){
    for(size_t m = 0; m < N; m++){
        re[m] = 0;
        im[m] = 0;
        for(size_t n = 0; n < N; n++){
            re[m] += x[n] * cos(2 * PI * ((float)m / N) * n);
            im[m] += x[n] * -sin(2 * PI * ((float)m / N) * n);
        }
    }
}