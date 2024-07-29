#include "temp_function.h"

int enter_data_bolt_diam(WINDOW *sub1, WINDOW *a1, int pair_num)
{
    int ch;
    char info[3];
    do
    {
        wclear(sub1);
        wbkgd(sub1, COLOR_PAIR(pair_num));
        box(stdscr, 0, 0);
        wmove(sub1, 0, 1);
        waddstr(sub1, "1. Enter bolt diameter: ");
        wgetnstr(sub1, info, 2);
        wmove(sub1, 1, 4);
        wprintw(sub1, "If the information is correct then press 'y', if incorrect press 'n' ");
        wmove(a1, 1, 45);   // работа с доп. окном a1
        wprintw(a1, "%s", info);
        wrefresh(a1);
        ch = wgetch(sub1);
        if (ch == 'n')
        {
            wmove(a1, 1, 45);
            waddrawch(a1, ' ');
            wmove(a1, 1, 46);
            waddrawch(a1, ' ');
            wrefresh(a1);
        }
    } while (ch != 'y');
    return atoi(info);
}

int enter_data_bolt_length(WINDOW *sub1, WINDOW *a1, int pair_num)
{
    int ch;
    char info[4];
    do
    {
        wclear(sub1);
        wbkgd(sub1, COLOR_PAIR(pair_num));
        box(stdscr, 0, 0);
        wmove(sub1, 0, 1);
        waddstr(sub1, "2. Enter bolt length: ");
        wgetnstr(sub1, info, 3);
        wmove(sub1, 1, 4);
        wprintw(sub1, "If the information is correct then press 'y', if incorrect press 'n' ");
        wmove(a1, 2, 45);   // работа с доп. окном a1
        wprintw(a1, "%s", info);
        wrefresh(a1);
        ch = wgetch(sub1);
        if (ch == 'n')
        {
            wmove(a1, 2, 45);
            waddrawch(a1, ' ');
            wmove(a1, 2, 46);
            waddrawch(a1, ' ');
            wmove(a1, 2, 47);
            waddrawch(a1, ' ');
            wrefresh(a1);
        }
    } while (ch != 'y');
    return atoi(info);
}

int enter_data_thick_parts_head(WINDOW *sub1, WINDOW *a1, int pair_num)
{
    int ch;
    char info[4];
    do
    {
        wclear(sub1);
        wbkgd(sub1, COLOR_PAIR(pair_num));
        box(stdscr, 0, 0);
        wmove(sub1, 0, 1);
        waddstr(sub1, "3. Thickness of parts under the head: ");
        wgetnstr(sub1, info, 3);
        wmove(sub1, 1, 4);
        wprintw(sub1, "If the information is correct then press 'y', if incorrect press 'n' ");
        wmove(a1, 4, 45);   // работа с доп. окном a1
        wprintw(a1, "%s", info);
        wrefresh(a1);
        ch = wgetch(sub1);
        if (ch == 'n')
        {
            wmove(a1, 4, 45);
            waddrawch(a1, ' ');
            wmove(a1, 4, 46);
            waddrawch(a1, ' ');
            wmove(a1, 4, 47);
            waddrawch(a1, ' ');
            wrefresh(a1);
        }
    } while (ch != 'y');
    return atoi(info);
}

int enter_data_thick_part_nut(WINDOW *sub1, WINDOW *a1, int pair_num)
{
    int ch;
    char info[4];
    do
    {
        wclear(sub1);
        wbkgd(sub1, COLOR_PAIR(pair_num));
        box(stdscr, 0, 0);
        wmove(sub1, 0, 1);
        waddstr(sub1, "4. Thickness of the part under the nut: ");
        wgetnstr(sub1, info, 3);
        wmove(sub1, 1, 4);
        wprintw(sub1, "If the information is correct then press 'y', if incorrect press 'n' ");
        wmove(a1, 5, 45);   // работа с доп. окном a1
        wprintw(a1, "%s", info);
        wrefresh(a1);
        ch = wgetch(sub1);
        if (ch == 'n')
        {
            wmove(a1, 5, 45);
            waddrawch(a1, ' ');
            wmove(a1, 5, 46);
            waddrawch(a1, ' ');
            wmove(a1, 5, 47);
            waddrawch(a1, ' ');
            wrefresh(a1);
        }
    } while (ch != 'y');
    return atoi(info);
}

int enter_data_number_wash_head(WINDOW *sub1, WINDOW *a1, int pair_num)
{
    int ch;
    char info[2];
    do
    {
        wclear(sub1);
        wbkgd(sub1, COLOR_PAIR(pair_num));
        box(stdscr, 0, 0);
        wmove(sub1, 0, 1);
        waddstr(sub1, "5. Number of washers under the bolt head: ");
        wgetnstr(sub1, info, 1);
        wmove(sub1, 1, 4);
        wprintw(sub1, "If the information is correct then press 'y', if incorrect press 'n' ");
        wmove(a1, 7, 45);   // работа с доп. окном a1
        wprintw(a1, "%s", info);
        wrefresh(a1);
        ch = wgetch(sub1);
        if (ch == 'n')
        {
            wmove(a1, 7, 45);
            waddrawch(a1, ' ');
            wrefresh(a1);
        }
    } while (ch != 'y');
    return atoi(info);
}

int enter_data_number_wash_nut(WINDOW *sub1, WINDOW *a1, int pair_num)
{
    int ch;
    char info[2];
    do
    {
        wclear(sub1);
        wbkgd(sub1, COLOR_PAIR(pair_num));
        box(stdscr, 0, 0);
        wmove(sub1, 0, 1);
        waddstr(sub1, "6. Number of washers under nuts: ");
        wgetnstr(sub1, info, 3);
        wmove(sub1, 1, 4);
        wprintw(sub1, "If the information is correct then press 'y', if incorrect press 'n' ");
        wmove(a1, 8, 45);   // работа с доп. окном a1
        wprintw(a1, "%s", info);
        wrefresh(a1);
        ch = wgetch(sub1);
        if (ch == 'n')
        {
            wmove(a1, 8, 45);
            waddrawch(a1, ' ');
            wrefresh(a1);
        }
    } while (ch != 'y');
    return atoi(info);
}