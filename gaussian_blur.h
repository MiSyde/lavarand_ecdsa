#ifndef LAVARAND_ECDSA_GAUSSIAN_BLUR_H
#define LAVARAND_ECDSA_GAUSSIAN_BLUR_H
#include <stdlib.h>

#include "math.h"
void gaussBlur(const float* source_channel, float* target_channel, int width, int height, float radius);

void boxBlur(const float* source_channel, float* target_channel, int width, int height, float radius);

void boxBlurHorizontal(const float* source_channel, float* target_channel, int width, int height, float radius);

void boxBlurTotal(const float* source_channel, float* target_channel, int width, int height, float radius);

float* boxesForGauss(float sigma, int n);
#endif //LAVARAND_ECDSA_GAUSSIAN_BLUR_H