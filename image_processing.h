#ifndef LAVARAND_ECDSA_IMAGE_PROCESSING_H
#define LAVARAND_ECDSA_IMAGE_PROCESSING_H

#include <stdbool.h>
#include "gaussian_blur.h"
#include "hsv.h"
#include "binary_processing.h"

unsigned char* process_img(const char* image_path, float floor_value, float ceiling_value, bool red);

#endif //LAVARAND_ECDSA_IMAGE_PROCESSING_H