#define BLOCKADES_INC
#ifndef STRUCTS_INC
    #include "structs.h"
#endif

_shape* create_shape(int hight, int width, int startx, int starty, int type);
int gen_start_coords(int num, int indx, int *s_y, int *s_x, int type);
void reinit_shape_coords(_shape_book *shape_book, int start, int end);
void rotate(_shape *shape);
void mirror(_shape *shape);
void shape_scroll(_shape_book *shape_book, int y_offset);
_shape_book* place_blocades(_shape_book *shape_book, int num, int per_line);
