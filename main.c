#include "stdio.h"
#include "fcntl.h"
#include "stdlib.h"

#include "window.h"
#include "client.h"

int main(int argc, char *argv[]){

    if (argc != 2){
        fprintf(stderr, "Usage: ./name 'file'");
        return 0;
    }

    // char *fdc = (char*)malloc((strlen(argv[1]) + 1)*sizeof(char));
    // strcpy(fdc, argv[1]);
    // fdc[strlen(argv[1])] = '\0';

    struct InitWindow window = {.SCREEN_HEIGHT = 800, .SCREEN_WIDTH = 600, .filename = NULL};

    window.filename = (char*)malloc((strlen(argv[1]) + 1)*sizeof(char));
    window.textbuffer = (char *)malloc(BUFFERSIZE * sizeof(char));

    if (TTF_Init() == -1) {
        fprintf(stderr, "TTF_Init error: %s\n", TTF_GetError());
        return 1;
    }

    window.text_font = TTF_OpenFont("Fonts/BigBlueTerm437NerdFont-Regular.ttf", 24);
    if (window.text_font == NULL) {
        fprintf(stderr, "Font error: %s\n", TTF_GetError());
        return 1;
    }

    strcpy(window.filename, argv[1]);

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init error: %s\n", SDL_GetError());
        return 1;
    }
    
    CreateWindow(&window);
    SDL_SetRenderDrawBlendMode(window.renderer, SDL_BLENDMODE_BLEND);

    run_client(&window);

    return 0;
}