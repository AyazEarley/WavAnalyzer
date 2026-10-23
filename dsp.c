#include "dsp.h"
#include <math.h>
#include <stdlib.h>

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

size_t dspZeroCrosses(const float *x, size_t N) {
    if (x == NULL) return 0;

    size_t count = 0;
    int prevSign = 0;

    for (size_t i = 0; i < N; i++) {
        float s = x[i];
        if (s == 0.0f) continue;

        int sign = (s > 0.0f) ? 1 : -1;
        if (prevSign != 0 && sign != prevSign) count++;
        prevSign = sign;
    }

    return count;
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

void dspRecursiveFFT(const float *in, size_t N, float *re, float *im){
    if (N == 1) { re[0] = in[0]; im[0] = 0; return; }

    float *evens = malloc(N / 2 * sizeof *evens);
    float *odds = malloc(N / 2 * sizeof *odds);

    float *evenRe = malloc(N / 2 * sizeof *evens);
    float *evenIm = malloc(N / 2 * sizeof *evens);
    float *oddRe = malloc(N / 2 * sizeof *odds);
    float *oddIm = malloc(N / 2 * sizeof *odds);

    assert(evens && odds && evenRe && evenIm && oddRe && oddIm);

    int evenIndex = 0;
    int oddIndex = 0;
    for(size_t i = 0; i < N; i++){
        if(i % 2 == 0){
            evens[evenIndex] = in[i];
            evenIndex++;
        }
        else{
            odds[oddIndex] = in[i];
            oddIndex++;
        }
    }

    dspRecursiveFFT(evens, N / 2, evenRe, evenIm);
    dspRecursiveFFT(odds, N / 2, oddRe, oddIm);
    

    for(size_t k = 0; k < N/2; k++){
        float angle = -2 * PI * (float)k / N;

        float tRe = cos(angle) * oddRe[k] - sin(angle) * oddIm[k];
        float tIm = cos(angle) * oddIm[k] + sin(angle) * oddRe[k];

        re[k] = evenRe[k] + tRe;
        re[k + N/2] = evenRe[k] - tRe;   

        im[k] = evenIm[k] + tIm;
        im[k + N/2] = evenIm[k] - tIm;
    }

    free(evens);
    free(odds);
    free(evenIm);
    free(evenRe);
    free(oddIm);
    free(oddRe);
}