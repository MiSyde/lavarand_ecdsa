#include "gaussian_blur.h"

void gaussBlur(const float* source_channel, float* target_channel, const int width, const int height, const float radius) {
    float* box = boxesForGauss(radius, 3);
    boxBlur(source_channel, target_channel, width, height, box[0]-(float)1/2);
    boxBlur(target_channel, source_channel, width, height, box[1]-(float)1/2);
    boxBlur(source_channel, target_channel, width, height, box[2]-(float)1/2);
    free(box);
}

void boxBlur(const float* source_channel, float* target_channel, const int width, const int height, const float radius) {
    const int size = width*height;
    for (int i = 0; i < size; ++i) {
        target_channel[i] = source_channel[i];
    }
    boxBlurHorizontal(target_channel, source_channel, width, height, radius);
    boxBlurTotal(source_channel, target_channel, width, height, radius);
}

void boxBlurHorizontal(const float* source_channel, float* target_channel, const int width, const int height, const float radius) {
    float iarr = 1 / (radius+radius+1);
    for (int i = 0; i < height; ++i) {
        int ti = i*width, li = ti, ri = ti+(int)radius;
        const float lv = source_channel[ti+width-1], fv = source_channel[ti];
        float value = (radius+1)*fv;

        for(int j = 0; j < (int)radius; ++j) {
            value += source_channel[ti+j];
        }

        for(int j = 0; j <= (int)radius; ++j) {
            value += source_channel[ri++] - fv;
            target_channel[ti++] = roundf(value*iarr);
        }

        for(int j = (int)radius+1; j < width-(int)radius; ++j) {
            value += source_channel[ri++] - source_channel[li++];
            target_channel[ti++] = roundf(value*iarr);
        }

        for(int j = width-(int)radius; j < width; ++j) {
            value += lv - source_channel[li++];
            target_channel[ti++] = roundf(value*iarr);
        }
    }
}

void boxBlurTotal(const float* source_channel, float* target_channel, const int width, const int height, const float radius) {
    float iarr = 1 / (radius+radius+1);
    for (int i = 0; i < width; ++i) {
        int ti = i, li = ti, ri = ti+(int)radius*width;
        const float lv = source_channel[ti+width*(height-1)], fv = source_channel[ti];
        float value = (radius+1)*fv;

        for(int j = 0; j < (int)radius; ++j) {
            value += source_channel[ti+j*width];
        }

        for(int j = 0; j <= (int)radius; ++j) {
            value += source_channel[ri] - fv;
            target_channel[ti] = roundf(value*iarr);
            ri += width;
            ti += width;
        }

        for(int j = (int)radius+1; j < height-(int)radius; ++j) {
            value += source_channel[ri] - source_channel[li];
            target_channel[ti] = roundf(value*iarr);
            li += width;
            ri += width;
            ti += width;
        }

        for(int j = height-(int)radius; j < height; ++j) {
            value += lv - source_channel[li];
            target_channel[ti] = roundf(value*iarr);
            li += width;
            ti += width;
        }
    }
}

float* boxesForGauss(const float sigma, const int n) {
    const float wIdeal = sqrtf((12*sigma*sigma/(float)n)+1);
    float wl = floorf(wIdeal);
    if ((int)wl % 2 == 0) --wl;
    const float wu = wl + 2;

    const float mIdeal = (12.0f*sigma*sigma - n*wl*wl - 3.0f*n) / (-4.0f*wl - 4.0f);
    const double m = roundf(mIdeal);

    float* sizes = malloc(sizeof(float)*n);
    for (int i = 0; i < n; ++i) {
        if (i < m) sizes[i] = wl;
        else sizes[i] = wu;
    }
    return sizes;
}