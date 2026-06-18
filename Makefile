CC      = gcc
CFLAGS  = -Wall -Wextra -g -I$(CURDIR)/include `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs` -lSDL2_ttf
 
SRC     = main.c src/client.c src/window.c
OBJ     = $(SRC:.c=.o)
EXEC    = editor
 
all: $(EXEC)
 
$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(LDFLAGS)
 
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
 
clean:
	rm -f $(OBJ) $(EXEC)
 