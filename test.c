#include <ncurses.h>
#include <cblas.h>
#include "utils.h"
#include <math.h>
#include <stdlib.h>


#define ALPHA 0.02454369260617026F /* pi/128 */

int test()
{

    /* Screens basis vectors */
    float scr_basis[8] = {
        1, 0, 0, 0,
        0, -1, 0, 0,
    };

    float parallel_lines[16] = {
        -10, -10, 0, 1,
        -10, 10, 0, 1,
        10, -10, 0, 1,
        10, 10, 0, 1
    };

    float R_z_positive[16] = {
        cos(ALPHA), -sin(ALPHA), 0, 0,
        sin(ALPHA), cos(ALPHA), 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };

    float R_z_negative[16] = {
        cos(ALPHA), sin(ALPHA), 0, 0,
        -sin(ALPHA), cos(ALPHA), 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };

    int screen_points[2*8];
    int c;

    float letter_scale = 1.5;

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    real_cords_to_screen_cords(stdscr, &scr_basis[0], &scr_basis[4], parallel_lines, screen_points, 2, letter_scale);
    real_cords_to_screen_cords(stdscr, &scr_basis[0], &scr_basis[4], &parallel_lines[4], &screen_points[2], 2, letter_scale);
    real_cords_to_screen_cords(stdscr, &scr_basis[0], &scr_basis[4], &parallel_lines[8], &screen_points[4], 2, letter_scale);
    real_cords_to_screen_cords(stdscr, &scr_basis[0], &scr_basis[4], &parallel_lines[12], &screen_points[6], 2, letter_scale);

    draw_figure(stdscr, screen_points, 2);
    draw_figure(stdscr, &screen_points[4], 2);

    while((c = getch()) != KEY_BACKSPACE) {
        switch(c) {
            case 'q':
                apply_map(&scr_basis[0], R_z_negative);
                apply_map(&scr_basis[4], R_z_negative);
        }

        clear();

        real_cords_to_screen_cords(stdscr, &scr_basis[0], &scr_basis[4], parallel_lines, screen_points, 2, letter_scale);
        real_cords_to_screen_cords(stdscr, &scr_basis[0], &scr_basis[4], &parallel_lines[4], &screen_points[2], 2, letter_scale);
        real_cords_to_screen_cords(stdscr, &scr_basis[0], &scr_basis[4], &parallel_lines[8], &screen_points[4], 2, letter_scale);
        real_cords_to_screen_cords(stdscr, &scr_basis[0], &scr_basis[4], &parallel_lines[12], &screen_points[6], 2, letter_scale);

        draw_figure(stdscr, screen_points, 2);
        draw_figure(stdscr, &screen_points[4], 2);

        refresh();

    }


    getch();
    endwin();

    return 0;
} 



