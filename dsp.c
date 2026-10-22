#include "dsp.h"
#include <math.h>

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
    if(n == 0) return 0.0;
    double total = 0;
    for(size_t i = 0; i < n; i++){
        total = total + (x[i] * x[i]);
    }
    total = total / n;
    total = sqrt(total);
    return (float)total;
}