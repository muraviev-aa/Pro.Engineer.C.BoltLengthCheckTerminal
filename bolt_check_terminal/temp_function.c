#include "temp_function.h"

void enter_data(WINDOW *sub1, char *info, char *arr, char *arr1, int pair_num)
{
    int ch;
    do
    {
        wclear(sub1);
        wbkgd(sub1, COLOR_PAIR(pair_num));
        box(stdscr, 0, 0);
        wmove(sub1, 0, 1);
        waddstr(sub1, arr);
        wgetnstr(sub1, info, 4);
        wmove(sub1, 1, 1);
        wprintw(sub1, arr1, info);
        wprintw(sub1, " If the information is correct then press 'y' ");
        ch = wgetch(sub1);
    } while (ch != 'y');
}