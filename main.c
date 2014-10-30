/*

 le Snake Mod : A mod of the classic snake game
                made for the *nix terminals

 Author     : 0poitr @ evilzone.org


 All the source files are released under GPLv3
 Please see the LICENSE file for terms and conditions.

 #Source files : main.c snake.c food.c menu.c blocade.c engine.c
 Run make to build executable.

*/

#include <unistd.h>
#include <stdlib.h>
#include <ncurses.h>
#include <signal.h>

#include "snake.h"
#include "engine.h"
#include "food.h"
#include "blocade.h"
#include "menu.h"

int SPEED=1;
int STATE = START;

void init_screen(void);
void init_game(_snake **snake, _food **food, _shape_book **shape_book);
void cleanup(_snake *snake, _food *food, _shape_book *shape_book);
void clr_n_exit(int sig);

void init_screen(void)
{
    initscr();
    cbreak();
    curs_set(0);
    keypad(stdscr, TRUE);
    noecho();
}

void check_start_color(void)
{
    if ( has_colors() ) {
        start_color();
        init_pair(BLUE_BLACK, COLOR_BLUE, COLOR_BLACK);
        init_pair(RED_BLACK, COLOR_RED, COLOR_BLACK);
        init_pair(CYAN_BLACK, COLOR_CYAN, COLOR_BLACK);
    }
}

void init_game(_snake **snake, _food **food, _shape_book **shape_book)
{
    *snake = NULL;
    *food = NULL;
    *shape_book = NULL;

    *snake = init_snake(5);
    *food = place_food(*food);
    *shape_book = place_blocades(*shape_book, 8, 2);

    if (!*snake || !*food || !*shape_book)
        { cleanup(*snake, *food, *shape_book); clr_n_exit(1); }
}

void cleanup(_snake *snake, _food *food, _shape_book *shape_book)
{
    if (snake) free(snake);
    if (food) free(food);
    int i=0;
    for (i=0; i<shape_book->t_blocade; i++)
        free(shape_book->shape_ptr[i]);

    free(shape_book);
}

void clr_n_exit(int sig)
{
    endwin();
    exit(sig);
}

int main(void)
{
    signal(SIGINT, clr_n_exit);

    srand( time(NULL) );
    init_screen();
    /*check_start_color();*/
    manage_menu();
    nodelay(stdscr, TRUE);

    _food *food;
    _snake *snake;
    _shape_book *shape_book;
    
    init_game(&snake, &food, &shape_book);
    draw_updates(snake, food, shape_book);

    unsigned int UPDATE_ITV = 150000U;

    int collide_indx, scroll=0, maxy, maxx;
    getmaxyx(stdscr, maxy, maxx);

    while (1) {
        int c = wgetch(stdscr);

        switch (c) {
            case ERR:
            break;

            case KEY_RIGHT: case 'd':
            snake_change_dir(snake, RIGHT);
            break;

            case KEY_LEFT: case 'a':
            snake_change_dir(snake, LEFT);
            break;

            case KEY_UP: case 'w':
            snake_change_dir(snake, UP);
            break;

            case KEY_DOWN: case 's':
            snake_change_dir(snake, DOWN);
            break;

            case 'q': STATE = END;
            show_score_win(snake->score);
            break;

            case 'p': case 'P':
            STATE = PAUSE;
            manage_menu();
            break;
        }
        
        if (STATE == END) break;

        if ( check_snake_collide(snake, shape_book, &collide_indx) ) {
            game_over(snake, collide_indx);
            STATE = END;
            manage_menu();
        }

        if (STATE == RESTART) {
            cleanup(snake, food, shape_book); 
            init_game(&snake, &food, &shape_book);
            draw_updates(snake, food, shape_book);
            STATE = START;
            continue;
        }

        if ( check_food_collect(snake, food) ) {
            grow_snake(snake);
            snake_update_score(snake, 1);
            food = place_food(food);
            if (!food) { cleanup(snake, food, shape_book); clr_n_exit(1); }
        }

        if (! (++scroll%2) ) {
            shape_scroll(shape_book, 1);
            snake_update_score(snake, 1);
        }
        if (scroll == maxy/2) {
            place_blocades(shape_book, 8, 2);
            scroll = 0;
        }

        update_snake(snake);
        draw_updates(snake, food, shape_book);

        usleep(UPDATE_ITV - (SPEED*10000));
    }
    
    cleanup(snake, food, shape_book); 
    clr_n_exit(0);
}
