#include <ncurses.h>
#include <cblas.h>
#include "utils.h"
#include <math.h>
#include <stdlib.h>

// #define TEST_PARALLEL_LINES
#define TEST_CUBE

#define ALPHA 0.02454369260617026F /* pi/128 */

int test()
{

    /* Screens basis vectors */

    #ifdef TEST_PARALLEL_LINES
    float scr_basis[8] = {
        1, 0, 0, 0,
        0, -1, 0, 0,
    };
    #endif

    #ifdef TEST_CUBE
    float scr_basis[8] = {
        -0.7071067811865475, 0.7071067811865475, 0, 0,
        0, 0.7071067811865475, -0.7071067811865475, 0
    };

    float scr_normal[4] = {
        -0.5773502691896258, -0.5773502691896258, -0.5773502691896258, 0
    };
    #endif

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

    float R_x_positive[16] = {
        1, 0, 0, 0,
        0, cos(ALPHA), -sin(ALPHA), 0,
        0, sin(ALPHA), cos(ALPHA), 0,
        0, 0, 0, 1
    };

    float cube[4*8] = {
        -10, -10, -10, 1,
        -10, 10, -10, 1,
        10, 10, -10, 1,
        10, -10, -10, 1,
        -10, -10, 10, 1,
        -10, 10, 10, 1,
        10, 10, 10, 1,
        10, -10, 10, 1
    };

    int screen_points[2*8];
    int c;
    int i, j, row, col;

    float letter_scale = 1.5;

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    #ifdef TEST_PARALLEL_LINES
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
    #endif

    getmaxyx(stdscr, row, col);
    float **z_buffer = calloc(row, sizeof(float *));
    i = 0;
    for(i; i < row; ++i){
        j = 0;
        z_buffer[i] = calloc(col, sizeof(float));
        for(j; j < col; j++)
        z_buffer[i][j] = __FLT32_MAX__;
    }
    

    i = 0;
    j = 0;
    for(i; i < 4*8; i += 4, j += 2)
        real_cords_to_screen_cords(stdscr, &scr_basis[0], &scr_basis[4], &cube[i], &screen_points[j], 2, letter_scale);

    draw_figure(stdscr, screen_points, 4);
    draw_figure(stdscr, &screen_points[2*4], 4);
    
    // draw_segment(stdscr, screen_points[0], screen_points[1], screen_points[2*4], screen_points[2*4 + 1]);
    // draw_segment(stdscr, screen_points[2], screen_points[3], screen_points[2*4 + 2], screen_points[2*4 + 3]);
    // draw_segment(stdscr, screen_points[4], screen_points[5], screen_points[2*4 + 4], screen_points[2*4 + 5]);
    // draw_segment(stdscr, screen_points[6], screen_points[7], screen_points[2*4 + 6], screen_points[2*4 + 7]);

    while ((c = getch()) != KEY_BACKSPACE) {
        switch (c) {
            case 'q':
                i = 0;
                for (i; i < 4*8; i += 4)
                    apply_map(&cube[i], R_z_positive);
                break;

            case 'r':
                i = 0;
                for (i; i < 4*8; i += 4)
                    apply_map(&cube[i], R_x_positive);
                break;
        }

        // i = 0;
        // j = 0;
        // for(i; i < 4*8; i += 4, j += 2)
        //     real_cords_to_screen_cords(stdscr, &scr_basis[0], &scr_basis[4], &cube[i], &screen_points[j], 2, letter_scale);

        clear();
        // draw_figure(stdscr, screen_points, 4);
        // draw_figure(stdscr, &screen_points[2*4], 4);
        
        // draw_segment(stdscr, screen_points[0], screen_points[1], screen_points[2*4], screen_points[2*4 + 1]);
        // draw_segment(stdscr, screen_points[2], screen_points[3], screen_points[2*4 + 2], screen_points[2*4 + 3]);
        // draw_segment(stdscr, screen_points[4], screen_points[5], screen_points[2*4 + 4], screen_points[2*4 + 5]);
        // draw_segment(stdscr, screen_points[6], screen_points[7], screen_points[2*4 + 6], screen_points[2*4 + 7]);

        i = 0;
        for(i; i < row; ++i){
            j = 0;
            for(j; j < col; j++)
            z_buffer[i][j] = __FLT32_MAX__;
        }

        fill_triangle_z(stdscr, &cube[16], &cube[20], &cube[24], &scr_basis[0], &scr_basis[4], scr_normal, z_buffer, 2, letter_scale);
        fill_triangle_z(stdscr, &cube[16], &cube[28], &cube[24], &scr_basis[0], &scr_basis[4], scr_normal, z_buffer, 2, letter_scale);

        fill_triangle_z(stdscr, &cube[0], &cube[4], &cube[8], &scr_basis[0], &scr_basis[4], scr_normal, z_buffer, 2, letter_scale);
        fill_triangle_z(stdscr, &cube[0], &cube[12], &cube[8], &scr_basis[0], &scr_basis[4], scr_normal, z_buffer, 2, letter_scale);

        fill_triangle_z(stdscr, &cube[0], &cube[4], &cube[16], &scr_basis[0], &scr_basis[4], scr_normal, z_buffer, 2, letter_scale);
        fill_triangle_z(stdscr, &cube[16], &cube[20], &cube[4], &scr_basis[0], &scr_basis[4], scr_normal, z_buffer, 2, letter_scale);

        fill_triangle_z(stdscr, &cube[4], &cube[8], &cube[20], &scr_basis[0], &scr_basis[4], scr_normal, z_buffer, 2, letter_scale);
        fill_triangle_z(stdscr, &cube[20], &cube[24], &cube[8], &scr_basis[0], &scr_basis[4], scr_normal, z_buffer, 2, letter_scale);

        fill_triangle_z(stdscr, &cube[8], &cube[12], &cube[24], &scr_basis[0], &scr_basis[4], scr_normal, z_buffer, 2, letter_scale);
        fill_triangle_z(stdscr, &cube[24], &cube[28], &cube[12], &scr_basis[0], &scr_basis[4], scr_normal, z_buffer, 2, letter_scale);

        fill_triangle_z(stdscr, &cube[12], &cube[16], &cube[28], &scr_basis[0], &scr_basis[4], scr_normal, z_buffer, 2, letter_scale);
        fill_triangle_z(stdscr, &cube[12], &cube[0], &cube[16], &scr_basis[0], &scr_basis[4], scr_normal, z_buffer, 2, letter_scale);
        

        i = 0;
        for (i; i < 12; i += 4)
            draw_segment_z(stdscr, &cube[i], &cube[i+4], &scr_basis[0], &scr_basis[4], scr_normal, z_buffer, 2, letter_scale);
        draw_segment_z(stdscr, &cube[0], &cube[12], &scr_basis[0], &scr_basis[4], scr_normal, z_buffer, 2, letter_scale);
        
        i = 16;
        for (i; i < 28; i += 4)
            draw_segment_z(stdscr, &cube[i], &cube[i+4], &scr_basis[0], &scr_basis[4], scr_normal, z_buffer, 2, letter_scale);
        draw_segment_z(stdscr, &cube[16], &cube[28], &scr_basis[0], &scr_basis[4], scr_normal, z_buffer, 2, letter_scale);

        draw_segment_z(stdscr, &cube[0], &cube[16], &scr_basis[0], &scr_basis[4], scr_normal, z_buffer, 2, letter_scale);
        draw_segment_z(stdscr, &cube[4], &cube[20], &scr_basis[0], &scr_basis[4], scr_normal, z_buffer, 2, letter_scale);
        draw_segment_z(stdscr, &cube[8], &cube[24], &scr_basis[0], &scr_basis[4], scr_normal, z_buffer, 2, letter_scale);
        draw_segment_z(stdscr, &cube[12], &cube[28], &scr_basis[0], &scr_basis[4], scr_normal, z_buffer, 2, letter_scale);

        refresh();
    }

    i = 0;
    for(i; i < row; i++)
        free(z_buffer[i]);
    free(z_buffer);


    getch();
    endwin();

    return 0;
} 



