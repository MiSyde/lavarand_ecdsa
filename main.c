#include "binary_processing.h"
#include "image_processing.h"
#include "camera_processing.h"

int main(const int argc, char** argv) {
    char input_mode[7];
    printf("--Choose input mode--\n1. Camera\n2. File\n> ");
    scanf("%s", input_mode);
    char image_path[256];

    if (strcmp(input_mode, "Camera") == 0 || strcmp(input_mode, "camera") == 0 || strcmp(input_mode, "C") == 0 || strcmp(input_mode, "c") == 0 || strcmp(input_mode, "1") == 0) {
        printf("--Choose mode--\n1. Auto\n2. Manual\n> ");
        char mode[8];
        scanf("%s", mode);
        AppState* app_state = NULL;
        if (Init((void**)&app_state, argc, argv) == SDL_APP_FAILURE) {
            return 1;
        }

        bool manual;
        if (strcmp(mode, "1") == 0 || strcmp(mode, "auto") == 0 || strcmp(mode, "Auto") == 0 || strcmp(mode, "a") == 0 || strcmp(mode, "A") == 0) {
            manual = false;
        } else {
            manual = true;
        }

        while (!app_state->saved) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                Event(app_state, &event);
            }
            if (Iterate(app_state, manual) == SDL_APP_FAILURE) break;
        }

        Quit(app_state, SDL_APP_SUCCESS);
        strcpy(image_path, "lavacam.png");
    } else {
        printf("--What's the file path of the image?--\n> ");
        scanf("%s", image_path);
    }

    float floor_value = 0, ceiling_value = 0;
    bool red = false;

    printf("--What color should the algorithm look for?--\n1. Red\n2. Orange\n3. Yellow\n4. Green\n5. Cyan\n6. Blue\n7. Purple\n8. Pink\n> ");
    char color_input[7];

    scanf("%s", color_input);
    if (strcmp(color_input,"R") == 0 || strcmp(color_input,"Red") == 0 || strcmp(color_input,"1") == 0 || strcmp(color_input,"red") == 0 || strcmp(color_input,"r") == 0) {
        red = true;
    } else if (strcmp(color_input,"O") == 0 || strcmp(color_input,"Orange") == 0 || strcmp(color_input,"2") == 0 || strcmp(color_input,"orange") == 0 || strcmp(color_input,"o") == 0) {
        floor_value = 20.0f;
        ceiling_value = 45.0f;
    } else if (strcmp(color_input,"Y") == 0 || strcmp(color_input,"Yellow") == 0 || strcmp(color_input,"3") == 0 || strcmp(color_input,"yellow") == 0 || strcmp(color_input,"y") == 0) {
        floor_value = 45.0f;
        ceiling_value = 70.0f;
    } else if (strcmp(color_input,"G") == 0 || strcmp(color_input,"Green") == 0 || strcmp(color_input,"4") == 0 || strcmp(color_input,"green") == 0 || strcmp(color_input,"g") == 0) {
        floor_value = 90.0f;
        ceiling_value = 150.0f;
    } else if (strcmp(color_input,"C") == 0 || strcmp(color_input,"Cyan") == 0 || strcmp(color_input,"5") == 0 || strcmp(color_input,"cyan") == 0 || strcmp(color_input,"c") == 0) {
        floor_value = 170.0f;
        ceiling_value = 200.0f;
    } else if (strcmp(color_input,"B") == 0 || strcmp(color_input,"Blue") == 0 || strcmp(color_input,"6") == 0 || strcmp(color_input,"blue") == 0 || strcmp(color_input,"b") == 0) {
        floor_value = 200.0f;
        ceiling_value = 260.0f;
    } else if (strcmp(color_input,"Pu") == 0 || strcmp(color_input,"Purple") == 0 || strcmp(color_input,"7") == 0 || strcmp(color_input,"purple") == 0 || strcmp(color_input,"pu") == 0) {
        floor_value = 260.0f;
        ceiling_value = 290.0f;
    } else if (strcmp(color_input,"Pi") == 0 || strcmp(color_input,"Pink") == 0 || strcmp(color_input,"8") == 0 || strcmp(color_input,"pink") == 0 || strcmp(color_input,"pi") == 0) {
        floor_value = 290.0f;
        ceiling_value = 330.0f;
    } else {
        printf("...");
        exit(404);
    }

    unsigned char* sha = process_img(image_path, floor_value, ceiling_value, red);
    print_sha(sha);

    free(sha);
    return 0;
}
