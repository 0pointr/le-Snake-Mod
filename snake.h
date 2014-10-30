#ifndef STRUCTS_INC
    #include "structs.h"
#endif

_snake* init_snake(int size);
void update_snake(_snake *snake);
void wrap_snake(_snake *snake);
void snake_change_dir(_snake *snake, int new_dir);
inline void snake_update_score(_snake *snake, int raise);
