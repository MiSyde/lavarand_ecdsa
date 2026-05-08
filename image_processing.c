#include "image_processing.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

unsigned char* process_img(const char* image_path, const float floor_value, const float ceiling_value, const bool red) {
    int width, height, channel;
    unsigned char* img = stbi_load(image_path, &width, &height, &channel, 0);

    const int size = width * height;

    float* mask = malloc(sizeof(float) * size);
    float* blurred_mask = malloc(sizeof(float) * size);
    unsigned char* binary = malloc(sizeof(unsigned char) * size);

    enum channel ch;

    if (red) {
        for (int i = 0; i < size; ++i) {
            const struct hsv hsv = convert_rgb_to_hsv(img[i*channel], img[i*channel+1], img[i*channel+2], &ch);

            if (((hsv.hue >= 0.0f && hsv.hue <= 20.0f) || (hsv.hue >= 330.0f && hsv.hue <= 360.0f)) && hsv.saturation > 0.3f && hsv.value > 0.3f) {
                mask[i] = 1.0f;
            } else {
                mask[i] = 0.0f;
            }
        }
    } else {
        for (int i = 0; i < size; ++i) {
            const struct hsv hsv = convert_rgb_to_hsv(img[i*channel], img[i*channel+1], img[i*channel+2], &ch);

            if (hsv.hue >= floor_value && hsv.hue <= ceiling_value && hsv.saturation > 0.3f && hsv.value > 0.3f) {
                mask[i] = 1.0f;
            } else {
                mask[i] = 0.0f;
            }
        }
    }

    gaussBlur(mask, blurred_mask, width, height, 3.0f);

    for (int i = 0; i < size; ++i) {
        binary[i] = (blurred_mask[i] > 0.5f) ? 255 : 0;
    }

    unsigned char* packed_binary = pack(binary, size);

    int total_blocks;
    packed_binary = padding(packed_binary, size, &total_blocks);

    compress(packed_binary, total_blocks);

    stbi_write_png("bin_ll.png", width, height, 1, binary, width);
    stbi_image_free(img);

    free(mask);
    free(blurred_mask);
    free(binary);
    free(packed_binary);

    return concatenate();
}