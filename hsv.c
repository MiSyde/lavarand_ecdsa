#include "hsv.h"

float max(const float r, const float g, const float b, enum channel* channel) {
    float max = r;
    *channel = RED;
    if (g > max) {
        max = g;
        *channel = GREEN;
    }
    if (b > max) {
        max = b;
        *channel = BLUE;
    }
    return max;
}

inline float min(const float r, const float g, const float b) {
    float min = r;
    if (g < min) min = g;
    if (b < min) min = b;
    return min;
}


struct hsv convert_rgb_to_hsv(float r, float g, float b, enum channel* channel) {
    struct hsv hsv;

    r = r / (float) 255;
    g = g / (float) 255;
    b = b / (float) 255;

    const float cmax = max(r, g, b, channel);
    const float cmin = min(r, g, b);

    const float delta = cmax - cmin;

    hsv.value = cmax;

    if (cmax == 0.0f) {
        hsv.saturation = 0.0f;
    } else {
        hsv.saturation = delta/cmax;
    }

    if (delta == 0.0f) {
        hsv.hue = 0.0f;
    } else {
        switch (*channel) {
            case RED:
                hsv.hue = 60.0f * ((g - b) / delta);
                if (hsv.hue < 0.0f) {
                    hsv.hue += 360.0f;
                }
                break;
            case GREEN:
                hsv.hue = 60.0f * ((b - r) / delta + 2);
                break;
            case BLUE:
                hsv.hue = 60.0f * ((r - g) / delta + 4);
                break;
            default:
                hsv.hue = 0.0f;
        }
    }

    return hsv;
}
