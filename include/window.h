#include "stdio.h"
#include "stdlib.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#define TITLE "Hs editor"
#define BUFFERSIZE 8192



struct MessageData
{
    char *text;
    SDL_Texture *textr;
};



struct InitWindow
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    int SCREEN_HEIGHT;
    int SCREEN_WIDTH;
    char *filename;
    char *textbuffer;
    TTF_Font *text_font;
    struct MessageData *msgs;
};

void CreateWindow(struct InitWindow *window);
