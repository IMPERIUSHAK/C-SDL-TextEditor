#include "stdlib.h"
#include "SDL2/SDL.h"

struct InitWindow
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    int SCREEN_HEIGHT;
    int SCREEN_WIDTH;
};

void CreateWindow(struct InitWindow *window);
