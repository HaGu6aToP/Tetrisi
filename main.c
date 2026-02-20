#include <ncurses.h>
#include <cblas.h>
#include "utils.h"
#include <math.h>
#include <stdlib.h>

int main (int nargs, char vargs[])
{
    /* Basis vectors */
    float ex[3] = {1, 0, 0};
    float ey[3] = {0, 1, 0};
    float ez[3] = {0, 0, 1};

    /* Screen basis */
    float gx[3] = {1, 0, 0};
    float gy[3] = {0, 1, 0};

    int c;

    initscr();
    cbreak();
    noecho();
    curs_set(0);              /* Hide cursor */
    keypad(stdscr, TRUE);

    int x0, y0, x1, y1;
    float point1[3] = {0, 0, 0};
    float point2[3] = {5, 12, 0};
    float point3[3] = {10, 0, 0};

    wprintw(stdscr, "Hello\n");

    getmaxyx(stdscr, y0, x0);
    x0 /= 2;
    y0 /= 2;
    x1 = 10;
    y1 = 10;
    draw_segment(stdscr, x0, y0, x1, y1);

    while ((c = getch()) != 'q') {
        
        switch (c) {
            case KEY_LEFT:
                --x1;
                break;
            case KEY_RIGHT:
                ++x1;
                break;
            case KEY_UP:
                --y1;
                break;
            case KEY_DOWN:
                ++y1;
                break;
        }

        clear();
        draw_segment(stdscr, x0, y0, x1, y1);
        // printw("%c", c);
        refresh();
    }
    

    // real_cords_to_screen_cords(stdscr, gx, gy, point1, &x0, &y0, 2);
    // real_cords_to_screen_cords(stdscr, gx, gy, point2, &x1, &y1, 2);  
    // draw_segment(stdscr, x0, y0, x1, y1);  

    // printw("%d %d", x, y);

    getch();

    endwin();
    
    

    return 0;
} 
