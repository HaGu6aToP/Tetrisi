#include <ncurses.h>

void draw_point(
    WINDOW *win,
    float *point,
    float *n                       /* Screen surface normal */
);

void draw_coordinate_system(
    WINDOW *win, 
    int space_dim                  /* Number of drawen basis vectors */
);

void draw_segment(WINDOW *win, int x0, int y0, int x1, int y1);

void real_cords_to_screen_cords(WINDOW *win, float *ex, float *ey, float *bias ,float *point, int *ipoint, int xscale, float letter_scale);

void rotate_screen(float *ex, float *ey, float *A);


/* Draw a figure consisted of n points. Points are storaged as a row */
void draw_figure(
    WINDOW *win,
    int *data,        /* Row style data of figures points */
    int n               /* Amount of points belongs to figure */
);
