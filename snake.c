/*

 le Snake Mod : A mod of the classic snake game
                made for the *nix terminals

 Author     : 0poitr @ evilzone.org


 All the source files are released under GPLv3
 Please see the LICENSE file for terms and conditions.

 #Source files : main.c snake.c food.c menu.c blocade.c engine.c
 Run make to build executable.

*/

#include <stdlib.h>
#include <ncurses.h>

#include "snake.h"

_snake* init_snake(int size)
{
    _snake *snake = malloc(sizeof(_snake));
    
    if (snake) {
        snake->size = size;
        snake->direction = RIGHT;
        snake->score = 0;

        int i, x, y;
        getmaxyx(stdscr, y, x);
        snake->body[0].y = y/2;
        snake->body[0].x = (x/2) - 10;
        snake->body[0].bc = 'x';

        for (i=1; i<snake->size; i++) {
            snake->body[i].y = y/2;
            snake->body[i].x = snake->body[i-1].x+1;
            snake->body[i].bc = 'x';
        }

        return snake;
    }
    else
        return NULL;
}

void update_snake(_snake *snake)
{
    int i;
    for (i=0; i<snake->size-1; i++) {
        snake->body[i].y = snake->body[i+1].y;
        snake->body[i].x = snake->body[i+1].x;
    }

    if (snake->direction == UP)
        snake->body[snake->size-1].y -= 1;

    if (snake->direction == DOWN)
        snake->body[snake->size-1].y += 1;

    if (snake->direction == LEFT)
        snake->body[snake->size-1].x -= 1;

    if (snake->direction == RIGHT)
        snake->body[snake->size-1].x += 1;

    wrap_snake(snake);
}

void wrap_snake(_snake *snake)
{
    int i, y, x;
    getmaxyx(stdscr, y, x);

    for (i=0; i<snake->size; i++) {
        if (snake->body[i].x > x)
            snake->body[i].x = snake->body[i].x-x;
        else if (snake->body[i].x < 0)
            snake->body[i].x = snake->body[i].x+x;

        if (snake->body[i].y > y)
            snake->body[i].y = snake->body[i].y-y;
        else if (snake->body[i].y < 0)
            snake->body[i].y = snake->body[i].y+y;
    }
}

void grow_snake(_snake *snake)
{
    if (snake->direction == RIGHT || snake->direction == LEFT)
        snake->body[snake->size].y = snake->body[snake->size-1].y;

    else if (snake->direction == DOWN || snake->direction == UP)
        snake->body[snake->size].x = snake->body[snake->size-1].x;

    if (snake->direction == RIGHT)
        snake->body[snake->size].x = snake->body[snake->size-1].x+1;

    else if (snake->direction == DOWN)
        snake->body[snake->size].y = snake->body[snake->size-1].y+1;

    else if (snake->direction == LEFT)
        snake->body[snake->size].x = snake->body[snake->size-1].x-1;

    else if (snake->direction == UP)
        snake->body[snake->size].y = snake->body[snake->size-1].y-1;
    
    snake->body[snake->size].bc = 'x';
    snake->size++;

    wrap_snake(snake);
}

void snake_change_dir(_snake *snake, int new_dir)
{
    if ( ( snake->direction == LEFT && new_dir == RIGHT ) ||
         ( snake->direction == RIGHT && new_dir == LEFT ) ||
         ( snake->direction == UP && new_dir == DOWN    ) ||
         ( snake->direction == DOWN && new_dir == UP    )
       ) {
        int i;
        for (i=0; i< snake->size/2; i++) {
            
            int tmp_y = snake->body[i].y;
            int tmp_x = snake->body[i].x;

            snake->body[i].y = snake->body[snake->size-1 -i].y;
            snake->body[i].x = snake->body[snake->size-1 -i].x;

            snake->body[snake->size-1 -i].y = tmp_y;
            snake->body[snake->size-1 -i].x = tmp_x;
        }
    }
    snake->direction = new_dir;
}

inline void snake_update_score(_snake *snake, int raise)
{
    snake->score += raise;
}
