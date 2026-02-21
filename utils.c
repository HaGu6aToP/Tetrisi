#include "utils.h"
#include <stdlib.h>
#include <math.h>
#include <cblas.h>

#define draw_line_bresenham(win, x0, y0, x1, y1) draw_segment(win, x0, y0, x1, y1);

void swap(int *a, int *b) { int t = *a; *a = *b; *b = t; }
void swapf(float *a, float *b) { float t = *a; *a = *b; *b = t; }

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

void real_cords_to_screen_cords(WINDOW *win, float *ex, float *ey, float *point, int *ipoint, int xscale, float letter_scale)
{
    /* Map float cords in world basis to screen basis cords */
    float x, y;

    x = cblas_sdot(3, point, 1, ex, 1);
    y = cblas_sdot(3, point, 1, ey, 1);

    getmaxyx(win, ipoint[1], ipoint[0]);
    ipoint[0] /= 2;
    ipoint[1] /= 2;
    // *ix = 0;
    // *iy = 0;
    ipoint[0] += xscale * ((int)round(x) / letter_scale);
    ipoint[1] += ((int)round(y) / letter_scale);
}

void apply_map(float *v, float *A)
{
    float res[4];
    int i;

    cblas_sgemv(
        CblasRowMajor,
        CblasNoTrans,
        4, 4,
        1.0F,
        A,
        4,
        v,
        1,
        0.0F,
        res,
        1
    );


    for(i = 0; i < 3; ++i){
        v[i] = res[i];
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


void draw_segment_z(
    WINDOW *win,
    float *point0,
    float *point1,
    float *ex,
    float *ey,
    float *normal,
    float **z_buffer,
    int xscale, float letter_scale
)
{
    int x0, y0, x1, y1;
    float z0, z1;
    int row, col;
    int dx, dy;
    int sx, sy;
    int err, e2;
    int steps;
    int current_step;
    float t, cz;

    getmaxyx(win, row, col);

    x0 = x1 = col / 2;
    y0 = y1 = row / 2;

    x0 += xscale * ((int)round(cblas_sdot(3, point0, 1, ex, 1)) / letter_scale);
    x1 += xscale * ((int)round(cblas_sdot(3, point1, 1, ex, 1)) / letter_scale);

    y0 += (int)round(cblas_sdot(3, point0, 1, ey, 1)) / letter_scale;
    y1 += (int)round(cblas_sdot(3, point1, 1, ey, 1)) / letter_scale;

    z0 = cblas_sdot(3, point0, 1, normal, 1);
    z1 = cblas_sdot(3, point1, 1, normal, 1);

    // draw_segment(win, x0, y0, x1, y1);

    dx = abs(x1 - x0);
    dy = -abs(y1 - y0);
    sx = x0 < x1 ? 1 : -1;
    sy = y0 < y1 ? 1 : -1;
    err = dx + dy;
    e2;

    steps = (abs(dx) > abs(dy) ? abs(dx) : abs(dy));
    current_step = 0;

    while (1) {
        t = (steps == 0) ? 1.0f : (float)current_step / steps;
        cz = z0 + t * (z1 - z0);

        if (y0 >= 0 && y0 < row && x0 >= 0 && x0 < col) {
            if (-20 <= cz && (cz-2) <= z_buffer[y0][x0] + 8) {
                mvwprintw(win, 0, 0, "OK");
                mvwaddch(win, y0, x0, '*');
                z_buffer[y0][x0] = cz;
            } 
        }

        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
    
}

void fill_triangle_z(WINDOW *win, 
    float *point0,
    float *point1,
    float *point2,
    float *ex,
    float *ey,
    float *normal,
    float **z_buffer,
    int xscale, float letter_scale
)
{
    int x, y;
    int x0, y0, x1, y1, x2, y2;
    int ax, bx;
    float z0, z1, z2;
    float az, bz;
    float t, current_z;
    int row, col;
    int total_height, segment_height;
    int i, j;
    bool second_half;
    float alpha, beta;

    // wattron(win, A_REVERSE);
    getmaxyx(win, row, col);

    x0 = x1 = x2 = col / 2;
    y0 = y1 = y2 = row / 2;

    x0 += xscale * ((int)round(cblas_sdot(3, point0, 1, ex, 1)) / letter_scale);
    x1 += xscale * ((int)round(cblas_sdot(3, point1, 1, ex, 1)) / letter_scale);
    x2 += xscale * ((int)round(cblas_sdot(3, point2, 1, ex, 1)) / letter_scale);

    y0 += (int)round(cblas_sdot(3, point0, 1, ey, 1)) / letter_scale;
    y1 += (int)round(cblas_sdot(3, point1, 1, ey, 1)) / letter_scale;
    y2 += (int)round(cblas_sdot(3, point2, 1, ey, 1)) / letter_scale;

    z0 = cblas_sdot(3, point0, 1, normal, 1);
    z1 = cblas_sdot(3, point1, 1, normal, 1);
    z2 = cblas_sdot(3, point2, 1, normal, 1);

    if (y0 > y1) { swap(&x0, &x1); swap(&y0, &y1); swapf(&z0, &z1); }
    if (y0 > y2) { swap(&x0, &x2); swap(&y0, &y2); swapf(&z0, &z2); }
    if (y1 > y2) { swap(&x1, &x2); swap(&y1, &y2); swapf(&z1, &z2); }

    total_height = y2 - y0;
    if (total_height == 0) return;

    i = 0;
    for (i; i <= total_height; ++i) {
        second_half = i > (y1 - y0) || y1 == y0;
        segment_height = second_half ? (y2 - y1) : (y1 - y0);
        if (segment_height == 0) segment_height = 1;

        alpha = (float)i / total_height;
        beta  = (float)(i - (second_half ? (y1 - y0) : 0)) / segment_height;
        ax = x0 + (x2 - x0) * alpha;
        az = z0 + (z2 - z0) * alpha;

        bx = second_half ? x1 + (x2 - x1) * beta : x0 + (x1 - x0) * beta;
        bz = second_half ? z1 + (z2 - z1) * beta : z0 + (z1 - z0) * beta;

        if (ax > bx) { swap(&ax, &bx); swapf(&az, &bz); }

        j = ax;
        for (j; j <= bx; ++j) {
            t = (ax == bx) ? 1.0f : (float)(j - ax) / (bx - ax);
            current_z = az + (bz - az) * t;

            // put_pixel_z(win, j, y0 + i, current_z, ch);
            x = j;
            y = y0 + i;

            if (y >= 0 && y < row && x >= 0 && x < col) {
                if (-20 <= current_z && current_z <= z_buffer[y][x]) {
                    mvwprintw(win, 0, 0, "OK");
                    mvwaddch(win, y, x, ' ');
                    z_buffer[y][x] = current_z;
                }
            }
        }
    }
    // wattroff(win, A_REVERSE);
}
