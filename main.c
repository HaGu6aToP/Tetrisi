#include <ncurses.h>
#include <cblas.h>
#include "utils.h"
#include <math.h>
#include <stdlib.h>

// #define draw_segment_TEST
// #define triangle_TEST
#define rotating_triangle_TEST
#define ALPHA 0.02454369260617026F /* pi/128 */

int main (int nargs, char vargs[])
{
    /* Basis vectors */
    float ex[4] = {1, 0, 0, 1};
    float ey[4] = {0, 1, 0, 1};
    float ez[4] = {0, 0, 1, 1};

    /* Screen basis */
    float gx[4] = {1, 0, 0, 1};
    float gy[4] = {0, -1, 0, 1};

    float R_z[16] = {cos(ALPHA), -sin(ALPHA), 0, 0, sin(ALPHA), cos(ALPHA), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};

    int c;

    initscr();
    cbreak();
    noecho();
    curs_set(0);              /* Hide cursor */
    keypad(stdscr, TRUE);

    int x0, y0, x1, y1, x2, y2;
    float point1[3] = {0, 0, 0};
    float point2[3] = {10, 24, 0};
    float point3[3] = {20, 0, 0};

    wprintw(stdscr, "Hello\n");

    #ifdef draw_segment_TEST
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

    clear();
    #endif

    #ifdef triangle_TEST
    real_cords_to_screen_cords(stdscr, gx, gy, point1, &x0, &y0, 2);
    real_cords_to_screen_cords(stdscr, gx, gy, point2, &x1, &y1, 2);
    real_cords_to_screen_cords(stdscr, gx, gy, point3, &x2, &y2, 2);

    draw_segment(stdscr, x0, y0, x1, y1);
    draw_segment(stdscr, x1, y1, x2, y2);
    draw_segment(stdscr, x0, y0, x2, y2);  

    // printw("%d %d", x, y);
    #endif

    
    #ifdef rotating_triangle_TEST
    while ((c = getch()) != KEY_BACKSPACE) {
        
        switch (c) {
            case 'q':
                rotate_screen(gx, gy, R_z);
                mvprintw(0, 0, "%.2f %.2f %.2f %.2f\n", gx[0], gx[1], gx[2], gx[3]);
                printw("%.2f %.2f %.2f %.2f\n", gy[0], gy[1], gy[2], gy[3]);
                break;
        }

        clear();
        real_cords_to_screen_cords(stdscr, gx, gy, point1, &x0, &y0, 2);
        real_cords_to_screen_cords(stdscr, gx, gy, point2, &x1, &y1, 2);
        real_cords_to_screen_cords(stdscr, gx, gy, point3, &x2, &y2, 2);

        draw_segment(stdscr, x0, y0, x1, y1);
        draw_segment(stdscr, x1, y1, x2, y2);
        draw_segment(stdscr, x0, y0, x2, y2);  
        // printw("%c", c);
        refresh();
    }
    #endif
    getch();

    endwin();
    
    return 0;
} 



