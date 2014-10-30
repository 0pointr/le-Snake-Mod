/*

 le Snake Mod : A mod of the classic snake game
                made for the *nix terminals

 Author     : 0poitr @ evilzone.org


 All the source files are released under GPLv3
 Please see the LICENSE file for terms and conditions.

 #Source files : main.c snake.c food.c menu.c blocade.c engine.c
 Run make to build executable.

*/

#include <ncurses.h>

#include "structs.h"
#include "engine.h"

bool check_food_collect(_snake *snake, _food *food)
{
    if ( ( food->y == snake->body[snake->size-1].y &&
           food->x == snake->body[snake->size-1].x  )
        ||  
         ( food->y == snake->body[0].y &&
           food->x == snake->body[0].x  )
       )
        return true;
    else
        return false;
}

bool collide_self(_snake *snake, int *indx)
{
    register int i, j;

    if ( snake->body[0].y == snake->body[snake->size-1].y &&
         snake->body[0].x == snake->body[snake->size-1].x )
        return true;

    for (i=1; i<snake->size-1; i++) {
        if ( snake->body[i].y == snake->body[0].y &&
             snake->body[i].x == snake->body[0].x )
        {
            *indx = 0;
            return true;
        }
    }

    for (i=0; i<snake->size-2; i++) {
        if ( snake->body[i].y == snake->body[snake->size-1].y && 
             snake->body[i].x == snake->body[snake->size-1].x )
        {
            *indx = snake->size-1;
            return true;
        }
    }
    
    return false;
}

bool collide_shape(_snake *snake, _shape_book *shape_book, int *indx)
{
    int i, j, k;

    for (i=0; i<shape_book->n_blocade; i++) {
       _shape *tmp = shape_book->shape_ptr[i];

       for (j=0; j<snake->size; j++) {

           if (tmp->type == SHAPE_BOX) {
               if (( snake->body[j].y == tmp->pt[0].y  && /* top */
                     snake->body[j].x <= tmp->pt[1].x  &&
                     snake->body[j].x >= tmp->pt[0].x )
                  ||
                    ( snake->body[j].y == tmp->pt[3].y  && /* bottom */
                      snake->body[j].x <= tmp->pt[2].x  &&
                      snake->body[j].x >= tmp->pt[3].x )
                  ||
                    ( snake->body[j].x == tmp->pt[0].x  && /* left */
                      snake->body[j].y <= tmp->pt[3].y  &&
                      snake->body[j].y >= tmp->pt[0].y )
                  ||
                    ( snake->body[j].x == tmp->pt[1].x  && /* right */
                      snake->body[j].y <= tmp->pt[2].y  &&
                      snake->body[j].y >= tmp->pt[1].y ) )
               {
                    *indx = j;
                    return true;
               }
           }
           else {
               if (( snake->body[j].y >= tmp->pt[1].y  && /* thses three are common for both */
                     snake->body[j].y <= tmp->pt[2].y  && /* L and L_M */
                      snake->body[j].x == tmp->pt[1].x )
                  ||
                    ( snake->body[j].y >= tmp->pt[4].y  &&
                      snake->body[j].y <= tmp->pt[3].y  &&
                      snake->body[j].x == tmp->pt[4].x )
                  ||
                    ( snake->body[j].y >= tmp->pt[0].y  &&
                      snake->body[j].y <= tmp->pt[5].y  &&
                      snake->body[j].x == tmp->pt[0].x ) )
                  {
                        *indx = j;
                       return true;
                  }
                else {
                    switch (tmp->type) {
                    case SHAPE_L:
                    if (( snake->body[j].x >= tmp->pt[1].x  &&
                          snake->body[j].x <= tmp->pt[0].x  &&
                          snake->body[j].y == tmp->pt[1].y )
                      ||
                        ( snake->body[j].x >= tmp->pt[2].x  &&
                          snake->body[j].x <= tmp->pt[3].x  &&
                          snake->body[j].y == tmp->pt[2].y )
                      ||
                        ( snake->body[j].x >= tmp->pt[4].x  &&
                          snake->body[j].x <= tmp->pt[5].x  &&
                          snake->body[j].y == tmp->pt[4].y )
                     )
                   {
                        *indx = j;
                       return true;
                   }
                   break;

                   case SHAPE_L_M:
                   if ( ( snake->body[j].x >= tmp->pt[0].x  &&
                          snake->body[j].x <= tmp->pt[1].x  &&
                          snake->body[j].y == tmp->pt[0].y )
                      ||
                        ( snake->body[j].x >= tmp->pt[3].x  &&
                          snake->body[j].x <= tmp->pt[2].x  &&
                          snake->body[j].y == tmp->pt[3].y )
                      ||
                        ( snake->body[j].x >= tmp->pt[5].x  &&
                          snake->body[j].x <= tmp->pt[4].x  &&
                          snake->body[j].y == tmp->pt[5].y )
                     )
                   {
                        *indx = j;
                       return true;
                   }
                   break;
               }
            }
          }
        }
    }
    return false;
}

bool check_snake_collide(_snake *snake, _shape_book *shape_book, int *indx)
{
    if ( collide_self(snake, indx) ) return true;
    if ( collide_shape(snake, shape_book, indx) ) return true;

    return false;
}

void game_over(_snake *snake, int indx)
{
    mvaddch(snake->body[indx].y, snake->body[indx].x, 'X');
    refresh();
    sleep(1);

    int x, y;
    int sty, stx;
    getmaxyx(stdscr, y ,x);
    WINDOW *g_o_win = newwin(20, 40, sty=(y/2)-(20/2), stx=(x/2)-(40/2));
    box(g_o_win, ACS_VLINE, ACS_HLINE);
    mvwaddstr(g_o_win, (20/2)-1, (40/2)-10, "Game Over!\n");
    mvwprintw(g_o_win, (20/2), (40/2)-10, "Score: %d", snake->score);
    wrefresh(g_o_win);
    sleep(3);
    delwin(g_o_win);
}

void show_score_win(int score)
{
    int x, y;
    int sty, stx;
    getmaxyx(stdscr, y ,x);
    WINDOW *score_win = newwin(20, 40, sty=(y/2)-(20/2), stx=(x/2)-(40/2));
    box(score_win, ACS_VLINE, ACS_HLINE);
    mvwprintw(score_win, (20/2)-1, (40/2)-10, "Your Score: %d", score);
    wrefresh(score_win);
    sleep(3);
    delwin(score_win);
}

void draw_snake(_snake *snake)
{
    int i;
    for (i=0; i<snake->size; i++)
        mvaddch(snake->body[i].y, snake->body[i].x, snake->body[i].bc);
}

void draw_food(_food *food)
{
    mvaddch(food->y, food->x, '0');
}

void draw_shape(_shape *shape)
{
    /*static char i='1';*/

    mvaddch(shape->pt[0].y, shape->pt[0].x, ACS_BULLET);
    mvaddch(shape->pt[1].y, shape->pt[1].x, ACS_BULLET);
    mvaddch(shape->pt[2].y, shape->pt[2].x, ACS_BULLET);
    mvaddch(shape->pt[3].y, shape->pt[3].x, ACS_BULLET);
    /*i = (i>'5') ? '1' : i;
    if (shape->type != SHAPE_BOX) {*/
        mvaddch(shape->pt[4].y, shape->pt[4].x, ACS_BULLET);
        mvaddch(shape->pt[5].y, shape->pt[5].x, ACS_BULLET);
    /*}*/
    
    switch (shape->type) {
        case SHAPE_L:
        mvhline(shape->pt[1].y, shape->pt[1].x+1, ACS_HLINE, shape->width-1);
        mvhline(shape->pt[4].y, shape->pt[4].x+1, ACS_HLINE, shape->width-5-1);
        mvhline(shape->pt[2].y, shape->pt[2].x+1, ACS_HLINE, 5-1);
        mvvline(shape->pt[1].y+1, shape->pt[1].x, ACS_VLINE, shape->hight-1);
        mvvline(shape->pt[4].y+1, shape->pt[4].x, ACS_VLINE, shape->hight-5-1);
        mvvline(shape->pt[0].y+1, shape->pt[0].x, ACS_VLINE, 5-1);
        break;

        case SHAPE_L_M:
        mvhline(shape->pt[0].y, shape->pt[0].x+1, ACS_HLINE, shape->width-1);
        mvhline(shape->pt[5].y, shape->pt[5].x+1, ACS_HLINE, shape->width-5-1);
        mvhline(shape->pt[3].y, shape->pt[3].x+1, ACS_HLINE, 5-1);
        mvvline(shape->pt[1].y+1, shape->pt[1].x, ACS_VLINE, shape->hight-1);
        mvvline(shape->pt[4].y+1, shape->pt[4].x, ACS_VLINE, shape->hight-5-1);
        mvvline(shape->pt[0].y+1, shape->pt[0].x, ACS_VLINE, 5-1);
        break;

        case SHAPE_L_90:
        mvhline(shape->pt[2].y, shape->pt[2].x+1, ACS_HLINE, shape->width-1);
        mvhline(shape->pt[3].y, shape->pt[3].x+1, ACS_HLINE, shape->width-5-1);
        mvhline(shape->pt[5].y, shape->pt[5].x+1, ACS_HLINE, 5-1);
        mvvline(shape->pt[1].y+1, shape->pt[1].x, ACS_VLINE, shape->hight-1);
        mvvline(shape->pt[2].y+1, shape->pt[2].x, ACS_VLINE, shape->hight-5-1);
        mvvline(shape->pt[4].y+1, shape->pt[4].x, ACS_VLINE, 5-1);
        break;

        case SHAPE_BOX:
        mvhline(shape->pt[0].y, shape->pt[0].x+1, ACS_HLINE, shape->width-1);
        mvhline(shape->pt[3].y, shape->pt[3].x+1, ACS_HLINE, shape->width-1);
        mvvline(shape->pt[0].y+1, shape->pt[0].x, ACS_VLINE, shape->hight-1);
        mvvline(shape->pt[1].y+1, shape->pt[1].x, ACS_VLINE, shape->hight-1);
        break;

    }
}

void draw_score(_snake *snake)
{
    int i, y, x;
    getmaxyx(stdscr, y, x);
    
    mvprintw(0, 2, "Score: %d", snake->score);
    for (i=0; i<x; i++)
        mvaddch(1, i, ACS_HLINE);
}

void draw_updates(_snake *snake, _food *food, _shape_book *shape_book)
{
    erase();
    draw_snake(snake);
    draw_food(food);
    draw_score(snake);

    int i;
    for (i=0; i<shape_book->n_blocade; i++)
        draw_shape(shape_book->shape_ptr[i]);
    refresh();
}
