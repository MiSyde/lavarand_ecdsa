#include "camera_processing.h"

SDL_AppResult Init(void** appstate, int argc, char** argv) {
    AppState* app_state = malloc(sizeof(AppState));
    *app_state = (AppState) {.width = 600, .height = 600, .saved = false, .screenshot = false};
    *appstate = app_state;

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_CAMERA)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("LavaCam", app_state->width, app_state->height, 0, &(app_state->window), &(app_state->renderer))) {
        SDL_Log("Couldn't initialize window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    app_state->devices = SDL_GetCameras(&app_state->camera_count);

    if (app_state->devices == NULL || app_state->camera_count == 0) {
        SDL_Log("No cameras detected %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    for (int i = 0; i < app_state->camera_count; ++i) {
        printf("%d: %s\n", i+1, SDL_GetCameraName(app_state->devices[i]));
    }

    int id;
    printf("Index of camera:\n> ");

    scanf("%d", &id);
    app_state->camera = SDL_OpenCamera(app_state->devices[id-1], NULL);
    if (app_state->camera == NULL) {
        SDL_Log("Can't open selected camera: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult Iterate(void* appstate, const bool manual) {
    AppState* app_state = appstate;

    SDL_Surface* frame = SDL_AcquireCameraFrame(app_state->camera, NULL);

    if (frame != NULL) {
        app_state->frame_count++;
        if (app_state->texture == NULL) {
            SDL_SetWindowSize(app_state->window, frame->w, frame->h);
            app_state->height = frame->h;
            app_state->width = frame->w;
            app_state->texture = SDL_CreateTexture(app_state->renderer, frame->format,SDL_TEXTUREACCESS_STREAMING, frame->w, frame->h);
        }
        SDL_UpdateTexture(app_state->texture, NULL, frame->pixels, frame->pitch);
        if (manual) {
            if (app_state->frame_count >= 1 && app_state->screenshot) {
                if (strcmp(SDL_GetPixelFormatName(frame->format), "SDL_PIXELFORMAT_MJPG") == 0) {
                    const int jpeg_size = frame->pitch;
                    SDL_Surface* decoded = decode_MJPG(frame->pixels, jpeg_size);

                    if (decoded != NULL) {
                        SDL_SavePNG(decoded, "lavacam.png");
                        SDL_DestroySurface(decoded);
                    } else {
                        SDL_Log("MJPG decode failed");
                    }
                } else {
                    SDL_SavePNG(frame, "lavacam.png");
                }
                app_state->screenshot = false;
                app_state->saved = true;
            }
        } else {
            if (app_state->frame_count >= 1) {
                if (strcmp(SDL_GetPixelFormatName(frame->format), "SDL_PIXELFORMAT_MJPG") == 0) {
                    const int jpeg_size = frame->pitch;
                    SDL_Surface* decoded = decode_MJPG(frame->pixels, jpeg_size);

                    if (decoded != NULL) {
                        SDL_SavePNG(decoded, "lavacam.png");
                        SDL_DestroySurface(decoded);
                    } else {
                        SDL_Log("MJPG decode failed");
                    }
                } else {
                    SDL_SavePNG(frame, "lavacam.png");
                }
                app_state->saved = true;
            }
        }

        SDL_ReleaseCameraFrame(app_state->camera, frame);
    }

    SDL_SetRenderDrawColorFloat(app_state->renderer, 0.0f, 0.0f, 0.0f, SDL_ALPHA_OPAQUE_FLOAT);
    SDL_RenderClear((app_state->renderer));

    if (app_state->texture) {
        SDL_RenderTexture(app_state->renderer, app_state->texture, NULL, NULL);
    }

    SDL_RenderPresent(app_state->renderer);
    return SDL_APP_CONTINUE;
}

SDL_AppResult Event(void* appstate, const SDL_Event* event) {
    AppState* app_state = appstate;
    if (event->type == SDL_EVENT_QUIT) return SDL_APP_SUCCESS;
    if (event->type == SDL_EVENT_KEY_DOWN) {
        if (event->key.scancode == SDL_SCANCODE_T) {
            app_state->screenshot = true;
        }
    }
    return SDL_APP_CONTINUE;
}

void Quit(void* appstate, SDL_AppResult result) {
    AppState* app_state = appstate;
    if (app_state->devices != NULL) {
        SDL_free(app_state->devices);
    }
    if (app_state->camera != NULL) {
        SDL_CloseCamera(app_state->camera);
    }
    if (app_state->texture != NULL) {
        SDL_DestroyTexture(app_state->texture);
    }
    if (app_state->window != NULL) {
        SDL_DestroyWindow(app_state->window);
    }
    if (app_state->renderer != NULL) {
        SDL_DestroyRenderer(app_state->renderer);
    }
    free(app_state);
}

SDL_Surface* decode_MJPG(void* data, int size) {
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    jpeg_mem_src(&cinfo, (unsigned char*)data, size);
    jpeg_read_header(&cinfo, TRUE);
    jpeg_start_decompress(&cinfo);

    int width = (int) cinfo.output_width;
    int height = (int) cinfo.output_height;
    int channels = cinfo.output_components; // 3 = RGB

    unsigned char* buffer = malloc(width * height * channels);

    while (cinfo.output_scanline < height) {
        unsigned char* row = buffer + cinfo.output_scanline * width * channels;
        jpeg_read_scanlines(&cinfo, &row, 1);
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

    SDL_Surface* from_buffer = SDL_CreateSurfaceFrom(width, height, SDL_PIXELFORMAT_RGB24, buffer, width * channels);

    SDL_Surface* owned = SDL_ConvertSurface(from_buffer, SDL_PIXELFORMAT_RGB24);

    SDL_DestroySurface(from_buffer);
    free(buffer);

    return owned;

}
