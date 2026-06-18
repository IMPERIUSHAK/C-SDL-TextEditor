#include "client.h"

SDL_Texture* create_texture(struct InitWindow *app){
    SDL_Color White = {255, 255, 255, 255};

    SDL_Surface* surfaceMessage = TTF_RenderText_Blended_Wrapped(
        app->text_font, app->currtext, White, app->SCREEN_WIDTH - 40
    );
    if (surfaceMessage == NULL){
        fprintf(stderr, "%s", SDL_GetError());
    }

    SDL_Texture* Message = SDL_CreateTextureFromSurface(app->renderer, surfaceMessage);
    if (Message == NULL){
        fprintf(stderr, "Error while creating Texture");
    }

    SDL_FreeSurface(surfaceMessage);

    app->text_rect.x = 20;
    app->text_rect.y = 20;
    app->text_rect.w = app->SCREEN_WIDTH - 40;
    SDL_QueryTexture(Message, NULL, NULL, NULL, &app->text_rect.h);

    return Message;
}

void save_file(struct InitWindow *app) {
    int fd = open(app->filename, O_WRONLY | O_TRUNC);
    if (fd == -1) {
        perror("open");
        return;
    }

    int len = strlen(app->textbuffer);
    int written = write(fd, app->textbuffer, len);
    if (written == -1) {
        perror("write");
    }

    close(fd);
}

void handle_text_input(struct InitWindow *app, SDL_Event *event) {

    strcat(app->textbuffer, event->text.text);
    strcpy(app->currtext, app->textbuffer);

    SDL_DestroyTexture(app->textTexture);
    app->textTexture = create_texture(app);
}

void handle_keydown(struct InitWindow *app, SDL_Event *event) {

    SDL_Keymod mod = SDL_GetModState();

    if (event->key.keysym.scancode == SDL_SCANCODE_S &&
        (mod & KMOD_CTRL)) {
        save_file(app);
    }

    if (event->key.keysym.scancode == SDL_SCANCODE_BACKSPACE) {
      
        int len = strlen(app->textbuffer);
      
        if (len > 0) {
            app->textbuffer[len - 1] = '\0';
            strcpy(app->currtext, app->textbuffer);
            SDL_DestroyTexture(app->textTexture);
            app->textTexture = create_texture(app);
        }
    }
    
    if (event->key.keysym.scancode == SDL_SCANCODE_RETURN) {
    
        strcat(app->textbuffer, "\n");
        strcpy(app->currtext, app->textbuffer);
        SDL_DestroyTexture(app->textTexture);
        app->textTexture = create_texture(app);
    
    }
}

void handle_mouse_wheel(struct InitWindow *app, SDL_Event *event)
{
    int len = strlen(app->textbuffer);
    char *temp = (char *)malloc((len + 1) * sizeof(char));

    int len2 = 0;
    int flag = 0;

    if (event->wheel.y < 0){
        for (int i = 0; i < len; i++){
            if (app->textbuffer[i] == '\n' && flag < app->scroll_offset){ flag++; continue; }
            if (flag >= app->scroll_offset){
                temp[len2++] = app->textbuffer[i];
            }
        }

        if (len2 > 0){
            temp[len2] = '\0';
            free(app->currtext);
            app->currtext = temp;
            app->scroll_offset++;
            SDL_DestroyTexture(app->textTexture);
            app->textTexture = create_texture(app);
        } else {
            free(temp);
        }
    }

    if (event->wheel.y > 0 && app->scroll_offset > 0){
        app->scroll_offset--;

        int skip = 0;
        for (int i = 0; i < len; i++){
            if (app->textbuffer[i] == '\n' && skip < app->scroll_offset){ skip++; continue; }
            if (skip >= app->scroll_offset){
                temp[len2++] = app->textbuffer[i];
            }
        }

        if (len2 > 0){
            temp[len2] = '\0';
            free(app->currtext);
            app->currtext = temp;
            SDL_DestroyTexture(app->textTexture);
            app->textTexture = create_texture(app);
        } else {
            free(temp);
        }
    }
}

void handleEvents(struct InitWindow *app, SDL_Event *events, bool *isRunning){
    while(SDL_PollEvent(events)){
        switch (events->type){
            case SDL_QUIT:
                *isRunning = false;
                break;
            case SDL_KEYDOWN:
                if (events->key.keysym.scancode == SDL_SCANCODE_ESCAPE){
                    *isRunning = false;
                }
                handle_keydown(app, events);
                break;
            case SDL_MOUSEWHEEL:
                handle_mouse_wheel(app, events);
                break;
            case SDL_TEXTINPUT:
                handle_text_input(app, events);
                break;
            default:
                break;
        }
    }
}

int getfilefd(char *str){
    int fd = open(str, O_RDONLY);
    if (fd == -1) perror("open");
    return fd;
}

bool get_text(int fd, char **str){
    int n = 0;
    int offset = 0;

    while ((n = read(fd, *str + offset, BUFFERSIZE - offset - 1)) > 0){
        offset += n;
    }

    if (n == -1){
        perror("read");
        return false;
    }

    (*str)[offset] = '\0';
    return true;
}

void run_client(struct InitWindow *app){
    bool isRunning = true;
    SDL_Event events;

    int fd = getfilefd(app->filename);
    if (fd == -1){
        fprintf(stderr, "filename was: %s\n", app->filename);
        exit(-1);
    }

    if (!get_text(fd, &app->textbuffer)){
        fprintf(stderr, "Error reading file\n");
        exit(-1);
    }
    
    SDL_StartTextInput();
    strcpy(app->currtext, app->textbuffer);
    app->textTexture = create_texture(app);

    while (isRunning)
    {
        handleEvents(app, &events, &isRunning);

        SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
        SDL_RenderClear(app->renderer);

        SDL_Rect src = {0, 0, app->text_rect.w, app->text_rect.h};
        SDL_Rect dst = {20, 20, app->text_rect.w, app->text_rect.h};

        if (dst.y + dst.h > app->SCREEN_HEIGHT){
            dst.h = app->SCREEN_HEIGHT - dst.y;
            src.h = dst.h;
        }

        SDL_RenderCopy(app->renderer, app->textTexture, &src, &dst);
        SDL_RenderPresent(app->renderer);
    }

    SDL_StopTextInput();
    cleanMemory(app, fd);
}

void cleanMemory(struct InitWindow *app, int fd){
    close(fd);
    
    TTF_CloseFont(app->text_font);
    TTF_Quit();
    SDL_DestroyTexture(app->textTexture);
    SDL_DestroyRenderer(app->renderer);
    SDL_DestroyWindow(app->window);

    free(app->filename);
    free(app->textbuffer);
    free(app->currtext);
}