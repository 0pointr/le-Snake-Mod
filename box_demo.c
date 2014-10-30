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

#include "box.h"
#include "blocade.h"

void move_shape(_shape_book *s_book)
{
    int i, j;
    
    for (i=0; i<s_book->t_blocade; i++) {
        s_book->shape_ptr[i]->checked = false;
    }
    for (j=0; j<s_book->t_blocade; j++) {
        collision(s_book, j);
        for (i=0; i<4; i++) {
            s_book->shape_ptr[j]->pt[i].y += s_book->shape_ptr[j]->vector.y;
            s_book->shape_ptr[j]->pt[i].x += s_book->shape_ptr[j]->vector.x;
        }
    }
}

void collision(_shape_book *shape_book, int w)
{
    int x, y, i, j;
    getmaxyx(stdscr, y, x);
 
    /*for (i=0; i<shape_book->t_blocade; i++) {
        shape_book->shape_ptr[i]->checked = false;
    }
    for (i=0; i<shape_book->t_blocade; i++) {*/
        _shape *tmp = shape_book->shape_ptr[w];
        /* boundary check */
        if (tmp->pt[2].y >= y || tmp->pt[0].y <= 0)
            { tb_reflect(tmp); tmp->checked = true; }

        if (tmp->pt[0].x <= 0 || tmp->pt[2].x >= x)
            { lr_reflect(tmp); tmp->checked = true; }
        /* other entities */
        for (j=0; j<shape_book->t_blocade; j++) {
            if (j==w) continue;
            _shape *op = shape_book->shape_ptr[j];
            
            if (
                (
                  (tmp->pt[0].y == op->pt[3].y) &&
                  (ABS(tmp->pt[0].x - op->pt[3].x) <= tmp->width)
                )
                ||
                (
                  (tmp->pt[3].y == op->pt[0].y) &&
                  (ABS(tmp->pt[3].x - op->pt[0].x) <= tmp->width)
                )
              )
            {
                if (!tmp->checked) {
                    tb_reflect(tmp);
                    tmp->checked = true; }
                if (!op->checked) {
                    tb_reflect(op);
                    op->checked = true; }
            }
            if (
                (
                  (tmp->pt[0].x == op->pt[1].x) &&
                  (ABS(tmp->pt[0].y - op->pt[1].y) <= tmp->hight)
                )
                ||
                (
                  (tmp->pt[1].x == op->pt[0].x) &&
                  (ABS(tmp->pt[1].y - op->pt[0].y) <= tmp->hight)
                )
              )
            {
                if (!tmp->checked) {
                    lr_reflect(tmp);
                    tmp->checked = true; }
                if (!op->checked) {
                    lr_reflect(op);
                    op->checked = true; }
            }

        }
    //}
}

void tb_reflect(_shape *shape)
{
    if (shape->vector.y/shape->vector.x < 0)
        clockw_reflect(shape);
    else
        cclockw_reflect(shape);
}

void lr_reflect(_shape *shape)
{
    if (shape->vector.y/shape->vector.x > 0)
        clockw_reflect(shape);
    else
        cclockw_reflect(shape);
}

void inline clockw_reflect(_shape *shape)
{
    int tmp;
    tmp = shape->vector.x;
    shape->vector.x = -shape->vector.y;
    shape->vector.y = tmp;
}
void inline cclockw_reflect(_shape *shape)
{
    int tmp;
    tmp = shape->vector.x;
    shape->vector.x = shape->vector.y;
    shape->vector.y = -tmp;
}

void draw_stuff(_shape_book *s_book)
{
    erase();
    int i;
    for (i=0; i<s_book->t_blocade; i++)
        draw_shape(s_book->shape_ptr[i]);
    refresh();
}

_shape_book* create_box_system(int num, int per_line)
{
    _shape_book *shape_book = calloc(sizeof(_shape_book), 1);
    shape_book->t_blocade = num;
    shape_book->n_blocade = num;
    shape_book->per_line = per_line;

    int i, j=0, starty, startx;

    for (i=0; i<num; i++) {
        gen_start_coords(per_line, j, &starty, &startx, SHAPE_BOX);
        shape_book->shape_ptr[i] = create_shape(8, 8, starty, startx, SHAPE_BOX);
        if (++j == per_line) j=0;
    }

    return shape_book;

}

int main(void)
{
    initscr();
    cbreak();
    curs_set(0);
    nodelay(stdscr, TRUE);

    sleep(1);

    int y, x;

    getmaxyx(stdscr, y, x);
 /*   _shape *box = create_shape_box(5, 5, y/2, x/2);
    draw_stuff(box);*/
    
    _shape_book *s_book = create_box_system(5, 2);
    draw_stuff(s_book);

    while (getch() != 'q') {
        move_shape(s_book);
        draw_stuff(s_book);
        usleep(95000);
    }
    
    endwin();   
    return 0;
}

