#include <ncurses.h>
#include <string.h>

int main()
{
    char message[] = "Hello World !!!\n";                   /* Message to be written on the screen */
    int row, col;                                           /* To store the number of rows and */
                                                            /* the number of columns */

    initscr();                                              /* Start curses mode */
    noecho();                                               /* Switch off echoing */
    raw();                                                  /* Disable buffering line */

    getmaxyx(stdscr, row, col);                             /* Get the number of rows and columns*/
                                               
    move(row / 2, (col - strlen(message)) / 2);
    printw("%s", message);                                  /* Print Hello World :O */

    refresh();                                              /* Print it on to the real screen */
    getch();                                                /* Wait for user input */
    endwin();                                               /* End curses mode */
    return 0;
}
