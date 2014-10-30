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
#include "food.h"

_food* place_food(_food *food)
{
    static int init=0;
    int y, x, r;
    getmaxyx(stdscr, y, x);
    
    if (!init || STATE == RESTART) {
        food = malloc(sizeof(food));
        if (!food) return NULL;
        init = 1;
    }

    r = rand();
    food->y = r % y < 12 ? r%y+12 : r%y; /* avaoid where shapes are
                                            generated               */
    food->x = r % x;
 
    return food;
}

