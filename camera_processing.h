#ifndef LAVARAND_ECDSA_CAMERA_PROCESSING_H
#define LAVARAND_ECDSA_CAMERA_PROCESSING_H
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_CameraID* devices;
    SDL_Camera* camera;
    SDL_Texture* texture;
    int width;
    int height;
    int camera_count;
    bool screenshot;
    int frame_count;
    bool saved;
} AppState;

SDL_AppResult Init(void** appstate, int argc, char** argv);

SDL_AppResult Iterate(void* appstate, bool manual);

SDL_AppResult Event(void* appstate, const SDL_Event* event);

void Quit(void* appstate, SDL_AppResult result);

SDL_Surface* decode_MJPG(void* data, int size);

#endif //LAVARAND_ECDSA_CAMERA_PROCESSING_H