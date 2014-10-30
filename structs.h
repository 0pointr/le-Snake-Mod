#define STRUCTS_INC
#define MAX_SIZE 50
#define MAX_BLOCADES 20

#define true 1
#define false 0

#define ABS(x) (x < 0 ? -(x) : x)

extern int SPEED;
extern int STATE;

enum directions { UP, DOWN, LEFT, RIGHT };
enum states { START, PAUSE, END, RESTART };
enum shapes { SHAPE_L, SHAPE_L_M, SHAPE_L_90, SHAPE_L_90_M, SHAPE_BOX };
enum colors { BLUE_BLACK, RED_BLACK, CYAN_BLACK };

struct body_parts {
    char bc;
    int y;
    int x;
};

struct snake_struct {
    struct body_parts body[MAX_SIZE];
    int size;
    int score;
    short direction;
};

struct food_struct {
    int y;
    int x;
};

struct co_ordinates {
    int y;
    int x;
};

struct shape_struct {
    struct co_ordinates pt[10];
    int width;
    int hight;
    struct co_ordinates vector;
    int type;
    int checked;
};

struct shape_book_struct {
    struct shape_struct *shape_ptr[MAX_BLOCADES];
    int n_blocade;
    int t_blocade;
    int per_line;
};

typedef struct body_parts _body;
typedef struct snake_struct _snake;
typedef struct food_struct _food;
typedef struct co_ordinates _coord;
typedef struct shape_struct _shape;
typedef struct shape_book_struct _shape_book;
