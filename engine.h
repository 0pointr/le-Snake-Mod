#ifndef STRUCTS_INC
    #include "structs.h"
#endif

bool check_food_collect(_snake *snake, _food *food);
bool check_snake_collide(_snake *snake, _shape_book *shape_book, int *indx);
bool collide_self(_snake *snake, int *indx);
bool collide_shape(_snake *snake, _shape_book *shape_book, int *indx);
void draw_updates(_snake *snake, _food *food, _shape_book *shape_book);
void draw_snake(_snake *snake);
void draw_food(_food *food);
void draw_shape(_shape *shape);
void draw_score(_snake *snake);
void game_over(_snake *snake, int indx);
