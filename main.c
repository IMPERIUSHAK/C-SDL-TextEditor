#include "stdio.h"
#include "fcntl.h"
#include "stdlib.h"

#include "window.h"
#include "client.h"

int main(){

    struct InitWindow window = {.SCREEN_HEIGHT = 800, .SCREEN_WIDTH = 600};
    
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init error: %s\n", SDL_GetError());
        return 1;
    }
    
    CreateWindow(&window);
    SDL_SetRenderDrawBlendMode(window.renderer, SDL_BLENDMODE_BLEND);

    run_client(&window);

    return 0;
}