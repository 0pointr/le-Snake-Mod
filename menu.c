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

#include "menu.h"

void manage_menu(void)
{
    int y, x, l=1;
    char c;

    getmaxyx(stdscr, y, x);
    WINDOW *m_win = newwin(20, 60, y/2-10, x/2-30);
    refresh();

    box(m_win, ACS_VLINE, ACS_HLINE);

    mvwaddstr(m_win, l, 3, "   __      ____          __         __  ___        __");
    mvwaddstr(m_win, ++l, 3, "  / /__   / __/__  ___ _/ /_____   /  |/  /__  ___/ /");
    mvwaddstr(m_win, ++l, 3, " / / -_) _\\ \\/ _ \\/ _ `/  '_/ -_) / /|_/ / _ \\/ _  /");
    mvwaddstr(m_win, ++l, 3, "/_/\\__/ /___/_//_/\\_,_/_/\\_\\\\__/ /_/  /_/\\___/\\_,_/");

    mvwaddstr(m_win, l+=2, 46, "By 0pointr");

    mvwaddstr(m_win, l+=2, 2, "Keys:");
    mvwaddstr(m_win, ++l, 5, "WASD or Arrow Keys to move snake.");
    mvwaddstr(m_win, ++l, 5, "p: Pause/Resume game");
    mvwaddstr(m_win, ++l, 5, "s: Start game");
    mvwaddstr(m_win, ++l, 5, "r: Restart game");
    mvwaddstr(m_win, ++l, 5, "q: Exit game");
    mvwaddstr(m_win, ++l, 2, "Choose Speed:");
    mvwaddstr(m_win, ++l, 5, "1 2 3 4 5");

    mvwaddstr(m_win, 18, 48, "Speed: ");
    mvwaddch(m_win, 18, 55, (SPEED+'0'));

    if (STATE == PAUSE)
        mvwaddstr(m_win, 18, 2, "PAUSED");

    wrefresh(m_win);

    bool endloop = false;
    while ( c = getch() ) {
        switch (c) {
            case 'P': case 'p':
                if (STATE == PAUSE) {
                    STATE = START;
                    endloop = true;
                }
                break;

            case 'S': case 's':
                if (STATE == START) endloop = true;
                break;

            case 'R': case 'r':
                STATE = RESTART;
                endloop = true;
                break;

            case 'Q': case 'q':
                STATE = END;
                endloop = true;
                break;

            case '1': case '2': case '3': case '4': case '5':
                SPEED = c - '0';
                mvwaddch(m_win, 18, 55, c);
                wrefresh(m_win);
                break;
        }
        if (endloop) break;
    }
    delwin(m_win);
    return;
}
