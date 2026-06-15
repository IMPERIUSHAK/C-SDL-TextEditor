#include "client.h"

void handleEvents(SDL_Event *events, bool *isRunning){
    while(SDL_PollEvent(events)){
        switch (events->type){
            case SDL_QUIT:
                *isRunning = false;
                break;
            case SDL_KEYDOWN:
                if (events->key.keysym.scancode == SDL_SCANCODE_ESCAPE){
                    *isRunning = false;
                }
                break;
            default:
                break;
        }
    }
}

int getfilefd(char *str){

    printf("%s\n", str);

    int fd = open(str, O_RDWR, 0);

    return fd;
}

bool get_text(int fd, char **str){

    int n = 0;
    int offset = 0;
    
    while ((n = read(fd, *str + offset, BUFFERSIZE - offset - 1)) > 0){
        write(1, *str + offset, n);
        offset += n;
    }
    
    if (n == -1){
        return false;
    }
    
    (*str)[offset] = '\0';
    return true;
}

SDL_Texture* create_texture(struct InitWindow *app){
    SDL_Color White = {255, 255, 255};
    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(app->text_font, app->textbuffer, White); 
    SDL_Texture* Message = SDL_CreateTextureFromSurface(app->renderer, surfaceMessage);
    SDL_FreeSurface(surfaceMessage);
    return Message;
}


void run_client(struct InitWindow *app){
    bool isRunning = true;
    SDL_Event events;

    int fd = getfilefd(app->filename);
    if (fd == -1){
        fprintf(stderr, "Error with opening file");
        exit(-1);
    }

    printf("%d\n", fd);

    
    if (!get_text(fd, &app->textbuffer)){
        fprintf(stderr, "Error with opening file");
        exit(-1);
    }
    
    SDL_Texture* Message = create_texture(app);
    SDL_Rect Message_rect;
    
    Message_rect.x = 0;
    Message_rect.y = 0;
    // Message_rect.w = app->SCREEN_WIDTH - 200;
    // Message_rect.h = app->SCREEN_HEIGHT - 200;
    
    SDL_QueryTexture(Message, NULL, NULL, &Message_rect.w, &Message_rect.h);
    
    while (isRunning)
    {
        handleEvents(&events, &isRunning);
        
        SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
        SDL_RenderClear(app->renderer);
        
        SDL_RenderCopy(app->renderer, Message, NULL, &Message_rect);
        
        SDL_RenderPresent(app->renderer);
    }
    
}