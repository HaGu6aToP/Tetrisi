#include "utils.h"
#include <stdlib.h>
#include <math.h>
#include <cblas.h>

#define draw_line_bresenham(win, x0, y0, x1, y1) draw_segment(win, x0, y0, x1, y1);

float Q_rsqrt(float number)
{
    long i;
    float x2, y;
    const float threehalfs = 1.5F;

    x2 = number * 0.5F;
    y = number;
    i = * (long *)&y;
    i = 0x5f3759df - (i >> 1);
    y = * (float *)&i;
    y = y * (threehalfs - (x2 * y * y));

    return y;
}

void draw_coordinate_system(WINDOW *win, int space_dim)
{
    int row, col;
    int cy, cx;
    int len;
    int i, j;

    getmaxyx(win, row, col);

    cy = (row - len) / 2;
    cx = (col - len) / 2;
    len = 4;

    mvwaddch(win, cy, cx, 'x');

    i = cx + 1;
    for (i; i < cx+2*len; ++i) {
        mvwaddch(win, cy, i, ACS_HLINE);
    }
    mvwaddch(win, cy, i, 'o');

    j = cy - 1;
    for (j; j > cy-len; --j) {
        mvwaddch(win, j, cx, ACS_VLINE);
    }
    mvwaddch(win, j, cx, 'o');
}

/* Cohen-Sutherland codes */
const int LEFT   = 1; 
const int RIGHT  = 2; 
const int BOTTOM = 4; 
const int TOP    = 8; 

/* Cohen-Sutherland code for point (x, y) */
int computeCode(int x, int y, int xmax, int ymax) {
    int code = 0;

    if (x < 0) code |= LEFT;
    else if (x >= xmax) code |= RIGHT;
    if (y < 0) code |= TOP;
    else if (y >= ymax) code |= BOTTOM;

    return code;
}

/* Cohen-Sutherland algorithm */
void draw_Cohen_Sutherland(WINDOW *win, int x0, int y0, int x1, int y1)
{
    int row, col;
    getmaxyx(win, row, col);
    
    int xmax = col - 1;
    int ymax = row - 1;

    int code0 = computeCode(x0, y0, col, row);
    int code1 = computeCode(x1, y1, col, row);
    bool accept = false;

    while (true) {
        if (!(code0 | code1)) {
            /* Segment on the screen */
            accept = true;
            break;
        } else if (code0 & code1) {
            /* Segment out of the screen*/
            break;
        } else {
            /* Segment cross over screen */
            int codeOut = code0 ? code0 : code1;
            int x, y;

            if (codeOut & TOP) {
                x = x0 + (int)((float)(x1 - x0) * (0 - y0) / (y1 - y0));
                y = 0;
            } else if (codeOut & BOTTOM) {
                x = x0 + (int)((float)(x1 - x0) * (ymax - y0) / (y1 - y0));
                y = ymax;
            } else if (codeOut & RIGHT) {
                y = y0 + (int)((float)(y1 - y0) * (xmax - x0) / (x1 - x0));
                x = xmax;
            } else if (codeOut & LEFT) {
                y = y0 + (int)((float)(y1 - y0) * (0 - x0) / (x1 - x0));
                x = 0;
            }

            if (codeOut == code0) {
                x0 = x; y0 = y;
                code0 = computeCode(x0, y0, col, row);
            } else {
                x1 = x; y1 = y;
                code1 = computeCode(x1, y1, col, row);
            }
        }
    }


    if (accept) {
        draw_line_bresenham(win, x0, y0, x1, y1);
    }
}


/* Bresenham algorithm */
void draw_segment(WINDOW *win, int x0, int y0, int x1, int y1)
{
    int row, col;

    getmaxyx(win, row, col);

    // printw("%d %d %d %d | %d %d", x0, y0, x1, y1, col, row);

    // x0 = x0 < 0 ? 0 : x0;
    // x0 = x0 > col ? col : x0;
    // x1 = x1 < 0 ? 0 : x1;
    // x1 = x1 > col ? col : x1;

    // y0 = y0 < 0 ? 0 : y0;
    // y0 = y0 > row ? row : y0;
    // y1 = y1 < 0 ? 0 : y1;
    // y1 = y1 > row ? row : y1;

    int dx = abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;
    int e2;

    while (1) {
        if (y0 >= 0 && y0 < row && x0 >= 0 && x0 < col) {
            mvwaddch(win, y0, x0, '*');
        }
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }

    }
}

void real_cords_to_screen_cords(WINDOW *win, float *ex, float *ey, float *bias, float *point, int *ipoint, int xscale, float letter_scale)
{
    /* Map float cords in world basis to screen basis cords */
    float x, y;
    float biased_point[3] = {point[0] - bias[0], point[1] - bias[1], point[2] - bias[2]};

    x = cblas_sdot(3, biased_point, 1, ex, 1);
    y = cblas_sdot(3, biased_point, 1, ey, 1);

    getmaxyx(win, ipoint[1], ipoint[0]);
    ipoint[0] /= 2;
    ipoint[1] /= 2;
    // *ix = 0;
    // *iy = 0;
    ipoint[0] += xscale * ((int)round(x) / letter_scale);
    ipoint[1] += ((int)round(y) / letter_scale);
}

void rotate_screen(float *ex, float *ey, float *A)
{
    float resx[4];
    float resy[4];
    int i;

    cblas_sgemv(
        CblasRowMajor,
        CblasNoTrans,
        4, 4,
        1.0F,
        A,
        4,
        ex,
        1,
        0.0F,
        resx,
        1
    );

    cblas_sgemv(
        CblasRowMajor,
        CblasNoTrans,
        4, 4,
        1.0F,
        A,
        4,
        ey,
        1,
        0.0F,
        resy,
        1
    );

    for(i = 0; i < 3; ++i){
        ex[i] = resx[i];
        ey[i] = resy[i];
    }

}


void draw_figure(WINDOW *win, int *data, int n)
{
    switch(n) {
        case 2:
            draw_segment(win, data[0], data[1], data[2], data[3]);
            break;
        case 3:
            draw_segment(win, data[0], data[1], data[2], data[3]);
            draw_segment(win, data[2], data[3], data[4], data[5]);
            draw_segment(win, data[0], data[1], data[4], data[5]);
            break;
        case 4:
            draw_segment(win, data[0], data[1], data[2], data[3]);
            draw_segment(win, data[2], data[3], data[4], data[5]);
            draw_segment(win, data[4], data[5], data[6], data[7]);
            draw_segment(win, data[0], data[1], data[6], data[7]);
            break;
    }
}
