CC=gcc
CFLAGS=-lncurses -O2

all : le_snake_mod
debug : CFLAGS=-lncurses -ggdb
debug : le_snake_mod

le_snake_mod : snake_mod.o

snake_mod.o : main.c snake.c food.c blocade.c engine.c menu.c \
              snake.h food.h blocade.h engine.h menu.h
	$(CC) $(CFLAGS) main.c snake.c food.c blocade.c engine.c menu.c -o le_snake_mod
	# built le_snake_mod executable

clean :
	rm le_snake_mod
