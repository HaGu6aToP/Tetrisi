#include <ncurses.h>
#include <cblas.h>
#include "utils.h"
#include <math.h>
#include <stdlib.h>

// #define draw_segment_TEST
// #define triangle_TEST
// #define rotating_triangle_TEST
#define ALPHA 0.02454369260617026F /* pi/128 */

int test()
{
    /* Basis vectors */
    float ex[4] = {1, 0, 0, 1};
    float ey[4] = {0, 1, 0, 1};
    float ez[4] = {0, 0, 1, 1};

    /* Screen basis */
    float gx[4] = {1, 0, 0, 1};
    float gy[4] = {0, 1, 0, 1};
    float bias[3] = {0, 0, 0};

    float R_z_plus[16] = {cos(ALPHA), -sin(ALPHA), 0, 0, sin(ALPHA), cos(ALPHA), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
    float R_z_minus[16] = {cos(ALPHA), sin(ALPHA), 0, 0, -sin(ALPHA), cos(ALPHA), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};

    int c;
    float letter_scale = 2;

    initscr();
    cbreak();
    noecho();
    curs_set(0);              /* Hide cursor */
    keypad(stdscr, TRUE);

    int x0, y0, x1, y1, x2, y2;
    float point1[3] = {0, 0, 0};
    float point2[3] = {10, 24, 0};
    float point3[3] = {20, 0, 0};
    int point[8];
    float rectangle[12] = {-5, 5, 0, 5, 5, 0, 5, -5, 0, -5, -5, 0};
    float paralel_lines[12] = {-10, -10, 0, -10, 10, 0, 10, -10, 0, 10, 10, 0};

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
                rotate_screen(gx, gy, R_z_minus);
                
                break;
            case 'r':
                rotate_screen(gx, gy, R_z_plus);

                break;
            case 'f':
                letter_scale *= 0.9375;
                break;
            case 'b':
                letter_scale /= 0.9375;
                break;
            case KEY_LEFT:
                bias[0] -= 1;
                break;
            case KEY_RIGHT:
                bias[0] += 1;
                break;
                

        }

        clear();
        real_cords_to_screen_cords(stdscr, gx, gy, point1, bias, &point[0], 2, letter_scale);
        real_cords_to_screen_cords(stdscr, gx, gy, point2, bias, &point[2], 2, letter_scale);
        real_cords_to_screen_cords(stdscr, gx, gy, point3, bias, &point[4], 2, letter_scale);

        draw_figure(stdscr, point, 3);

        // real_cords_to_screen_cords(stdscr, gx, gy, &rectangle[0], &point[0], 2, letter_scale);
        // real_cords_to_screen_cords(stdscr, gx, gy, &rectangle[3], &point[2], 2, letter_scale);
        // real_cords_to_screen_cords(stdscr, gx, gy, &rectangle[6], &point[4], 2, letter_scale);
        // real_cords_to_screen_cords(stdscr, gx, gy, &rectangle[9], &point[6], 2, letter_scale);

        // draw_figure(stdscr, point, 4);

        // real_cords_to_screen_cords(stdscr, gx, gy, &paralel_lines[0], bias, &point[0], 2, letter_scale);
        // real_cords_to_screen_cords(stdscr, gx, gy, &paralel_lines[3], bias, &point[2], 2, letter_scale);
        // real_cords_to_screen_cords(stdscr, gx, gy, &paralel_lines[6], bias, &point[4], 2, letter_scale);
        // real_cords_to_screen_cords(stdscr, gx, gy, &paralel_lines[9], bias, &point[6], 2, letter_scale);

        // attron(A_REVERSE);
        // draw_figure(stdscr, &point[0], 2);
        // draw_figure(stdscr, &point[4], 2);
        // attroff(A_REVERSE);

        // draw_segment(stdscr, x0, y0, x1, y1);
        // draw_segment(stdscr, x1, y1, x2, y2);
        // draw_segment(stdscr, x0, y0, x2, y2);  
        // printw("%c", c);
        mvprintw(0, 0, "%.2f %.2f %.2f %.2f\n", gx[0], gx[1], gx[2], gx[3]);
        printw("%.2f %.2f %.2f %.2f\n", gy[0], gy[1], gy[2], gy[3]);
        refresh();
    }
    #endif
    getch();

    endwin();
    
    return 0;
} 



