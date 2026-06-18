#include "window.h"
#include "stdbool.h"
#include "fcntl.h"
#include "stdio.h"
#include "unistd.h"

SDL_Texture* create_texture(struct InitWindow *app);
void cleanMemory(struct InitWindow *app, int fd);
int getfilefd(char *str);
void run_client(struct InitWindow *window);