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
#include <stdlib.h>

#include "blocade.h"

_shape* create_shape(int hight, int width, int starty, int startx, int type)
{
    _shape *new_shape = malloc(sizeof(_shape));
    if (!new_shape) return NULL;
    
    if (type == SHAPE_L) {
        new_shape->pt[0].x = startx+width;
        new_shape->pt[0].y = starty;
        new_shape->pt[1].x = startx;
        new_shape->pt[1].y = starty;
        new_shape->pt[2].x = startx;
        new_shape->pt[2].y = starty+hight;
        new_shape->pt[3].x = startx+5;
        new_shape->pt[3].y = starty+hight;
        new_shape->pt[4].x = startx+5;
        new_shape->pt[4].y = starty+5;
        new_shape->pt[5].x = startx+width;
        new_shape->pt[5].y = starty+5;

        new_shape->width = width;
        new_shape->hight = hight;
        new_shape->type = SHAPE_L;
    }
    else if (type == SHAPE_BOX) {
        new_shape->pt[0].x = startx;
        new_shape->pt[0].y = starty;
        new_shape->pt[1].x = startx+width;
        new_shape->pt[1].y = starty;
        new_shape->pt[2].x = startx+width;
        new_shape->pt[2].y = starty+hight;
        new_shape->pt[3].x = startx;
        new_shape->pt[3].y = starty+hight;

        new_shape->width = width;
        new_shape->hight = hight;

        new_shape->vector.x = 1;
        new_shape->vector.y = 1;
        new_shape->type = SHAPE_BOX;
    }

    return new_shape;
}

void reinit_shape_coords(_shape_book *shape_book, int start, int end)
{
    int startx, starty;
    int i, r;

    for (i=start; i<end; i++) {
        int width = shape_book->shape_ptr[i]->width;
        int hight = shape_book->shape_ptr[i]->hight;

        r = gen_start_coords(end-start, i-start, &starty, &startx, SHAPE_L);

        shape_book->shape_ptr[i]->pt[0].x = startx+width;
        shape_book->shape_ptr[i]->pt[0].y = starty;
        shape_book->shape_ptr[i]->pt[1].x = startx;
        shape_book->shape_ptr[i]->pt[1].y = starty;
        shape_book->shape_ptr[i]->pt[2].x = startx;
        shape_book->shape_ptr[i]->pt[2].y = starty+hight;
        shape_book->shape_ptr[i]->pt[3].x = startx+5;
        shape_book->shape_ptr[i]->pt[3].y = starty+hight;
        shape_book->shape_ptr[i]->pt[4].x = startx+5;
        shape_book->shape_ptr[i]->pt[4].y = starty+5;
        shape_book->shape_ptr[i]->pt[5].x = startx+width;
        shape_book->shape_ptr[i]->pt[5].y = starty+5;

        shape_book->shape_ptr[i]->type = SHAPE_L;
        if (! (r%2) )
            mirror(shape_book->shape_ptr[i]);
    }
}

int gen_start_coords(int num, int indx, int *s_y, int *s_x, int type)
{
    int x, y;
    getmaxyx(stdscr, y, x);
    int avail = x / num;

    int r1 = rand();
    if (type == SHAPE_L) (*s_y) = r1%2 - 3;
    else (*s_y) = r1%y - 3;

    (*s_x) = r1%avail + (indx*avail);

    if (! (r1%2) )
        (*s_x) = (*s_x) - avail*indx < 10 ? (*s_x) + (*s_x) - avail*indx : (*s_x);
    else
        (*s_x) = (*s_x) + 10 > avail*(indx+1) ? (*s_x) - ((*s_x)+10 - avail*(indx+1)) : (*s_x);

    return r1;
}

void rotate(_shape *shape)
{
    int i, tmp;

    switch (shape->type) {
        case SHAPE_L: SHAPE_L_90:
        for (i=0; i<6; i++) {
            if (i==1) continue;
            tmp = shape->pt[i].x;
            shape->pt[i].x = shape->pt[i].y;
            shape->pt[i].y = tmp;
        }
        if (shape->type == SHAPE_L)    shape->type = SHAPE_L_90;
        break;
    }
}

void mirror(_shape *shape)
{
    switch (shape->type) {
        case SHAPE_L:
        shape->pt[0].x = shape->pt[1].x - shape->pt[0].x + shape->pt[1].x;
        shape->pt[3].x = shape->pt[1].x - shape->pt[3].x + shape->pt[1].x;
        shape->pt[4].x = shape->pt[1].x - shape->pt[4].x + shape->pt[1].x;
        shape->pt[5].x = shape->pt[1].x - shape->pt[5].x + shape->pt[1].x;
        shape->type = SHAPE_L_M;
        break;

        case SHAPE_L_90:
        shape->pt[0].y = shape->pt[1].y - shape->pt[0].y + shape->pt[1].y;
        shape->pt[3].y = shape->pt[1].y - shape->pt[3].y + shape->pt[1].y;
        shape->pt[4].y = shape->pt[1].y - shape->pt[4].y + shape->pt[1].y;
        shape->pt[5].y = shape->pt[1].y - shape->pt[5].y + shape->pt[1].y;
        shape->type = SHAPE_L_90_M;
        break;
    }
}

void shape_scroll(_shape_book *shape_book, int y_offset)
{
    int i;
    for (i=0; i<shape_book->n_blocade; i++) {
        shape_book->shape_ptr[i]->pt[0].y += y_offset;
        shape_book->shape_ptr[i]->pt[1].y += y_offset;
        shape_book->shape_ptr[i]->pt[2].y += y_offset;
        shape_book->shape_ptr[i]->pt[3].y += y_offset;
        shape_book->shape_ptr[i]->pt[4].y += y_offset;
        shape_book->shape_ptr[i]->pt[5].y += y_offset;
    }
}


_shape_book* place_blocades(_shape_book *shape_book, int num, int per_line)
{
    static int tracker = 0;
    if (STATE == RESTART) tracker = 0;

    if ( shape_book && shape_book->n_blocade < shape_book->t_blocade ) {
        shape_book->n_blocade += shape_book->per_line;
        return shape_book;
    }
    else if (shape_book && shape_book->n_blocade == shape_book->t_blocade ) {
            reinit_shape_coords(shape_book, tracker, tracker+shape_book->per_line);
            tracker += shape_book->per_line;

            if (tracker == shape_book->t_blocade) tracker = 0;
            return shape_book;
    }


    shape_book = calloc(sizeof(_shape_book), 1);
    if (!shape_book) return NULL;

    shape_book->t_blocade = num;
    shape_book->n_blocade = per_line;
    shape_book->per_line = per_line;

    int i, j=0, r, startx, starty;

    for (i=0; i < shape_book->t_blocade; i++) {
        r = gen_start_coords(per_line, j, &starty, &startx, SHAPE_L);
        shape_book->shape_ptr[i] = create_shape(10, 10, starty, startx, SHAPE_L);
        if (! (r%2) ) mirror(shape_book->shape_ptr[i]);
        if (++j == per_line) j=0;
    }
    
    return shape_book;
}
