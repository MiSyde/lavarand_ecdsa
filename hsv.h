#ifndef LAVARAND_ECDSA_HSV_H
#define LAVARAND_ECDSA_HSV_H

enum channel {
    RED,
    GREEN,
    BLUE
};

struct hsv {
    float value;
    float saturation;
    float hue;
};

float max(float r, float g, float b, enum channel* channel);

float min(float r, float g, float b);

struct hsv convert_rgb_to_hsv(float r, float g, float b, enum channel* channel);

#endif //LAVARAND_ECDSA_HSV_H
