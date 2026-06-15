#include "window.h"
#include "stdbool.h"
#include "fcntl.h"
#include "stdio.h"
#include "unistd.h"

int getfilefd(char *str);
void run_client(struct InitWindow *window);