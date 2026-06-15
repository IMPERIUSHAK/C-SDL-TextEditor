#include "window.h"

void CreateWindow(struct InitWindow *window){

    window->window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                                window->SCREEN_HEIGHT, window->SCREEN_WIDTH, 0);
    
    if (!window->window){
        fprintf(stderr, "Error while creating window");
    }

    window->renderer = SDL_CreateRenderer(window->window, -1, 0);

}