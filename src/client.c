#include "client.h"

void eventHandler(SDL_Event *events, bool *isRunnig){

    while(SDL_PollEvent(events)){
        switch (events->type)
        {
        case SDL_QUIT:
            *isRunnig = false;
            break;
        
        default:
            break;
        }   
    }
}

void run_client(struct InitWindow *app){
    bool isRunning = true;
    SDL_Event events;

    while (isRunning)
    {
        eventHandler(&events, &isRunning);

        SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
        SDL_RenderClear(app->renderer);
        SDL_RenderPresent(app->renderer);
    }
    
}