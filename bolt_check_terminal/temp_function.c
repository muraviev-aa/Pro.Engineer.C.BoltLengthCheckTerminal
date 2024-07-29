#include "temp_function.h"

int enter_data_bolt_diam(WINDOW *sub1, WINDOW *a1, int pair_num)
{
    int ch;
    char info[4];
    do
    {
        wclear(sub1);
        wbkgd(sub1, COLOR_PAIR(pair_num));
        box(stdscr, 0, 0);
        wmove(sub1, 0, 1);
        waddstr(sub1, "1. Enter bolt diameter: ");
        wgetnstr(sub1, info, 3);
        wmove(sub1, 1, 1);
        wprintw(sub1, "Bolt diameter is %s.", info);
        wprintw(sub1, " If the information is correct then press 'y' ");
        wmove(a1, 5, 45);   // работа с доп. окном a1
        wprintw(a1, "%s", info);
        wrefresh(a1);
        ch = wgetch(sub1);
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
        wmove(sub1, 1, 1);
        wprintw(sub1, "Bolt length is %s.", info);
        wprintw(sub1, " If the information is correct then press 'y' ");
        wmove(a1, 7, 45);   // работа с доп. окном a1
        wprintw(a1, "%s", info);
        wrefresh(a1);
        ch = wgetch(sub1);
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
        wmove(sub1, 1, 1);
        wprintw(sub1, "Thickness of parts under the head is %s.", info);
        wprintw(sub1, " If the information is correct then press 'y' ");
        wmove(a1, 9, 45);   // работа с доп. окном a1
        wprintw(a1, "%s", info);
        wrefresh(a1);
        ch = wgetch(sub1);
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
        wmove(sub1, 1, 1);
        wprintw(sub1, "Thickness of the part under the nut is %s.", info);
        wprintw(sub1, " If the information is correct then press 'y' ");
        wmove(a1, 11, 45);   // работа с доп. окном a1
        wprintw(a1, "%s", info);
        wrefresh(a1);
        ch = wgetch(sub1);
    } while (ch != 'y');
    return atoi(info);
}

int enter_data_number_wash_head(WINDOW *sub1, WINDOW *a1, int pair_num)
{
    int ch;
    char info[4];
    do
    {
        wclear(sub1);
        wbkgd(sub1, COLOR_PAIR(pair_num));
        box(stdscr, 0, 0);
        wmove(sub1, 0, 1);
        waddstr(sub1, "5. Number of washers under the bolt head: ");
        wgetnstr(sub1, info, 3);
        wmove(sub1, 1, 1);
        wprintw(sub1, "Number of washers under the bolt head is %s.", info);
        wprintw(sub1, " If the information is correct then press 'y' ");
        wmove(a1, 13, 45);   // работа с доп. окном a1
        wprintw(a1, "%s", info);
        wrefresh(a1);
        ch = wgetch(sub1);
    } while (ch != 'y');
    return atoi(info);
}

int enter_data_number_wash_nut(WINDOW *sub1, WINDOW *a1, int pair_num)
{
    int ch;
    char info[4];
    do
    {
        wclear(sub1);
        wbkgd(sub1, COLOR_PAIR(pair_num));
        box(stdscr, 0, 0);
        wmove(sub1, 0, 1);
        waddstr(sub1, "6. Number of washers under nuts: ");
        wgetnstr(sub1, info, 3);
        wmove(sub1, 1, 1);
        wprintw(sub1, "Number of washers under nuts is %s.", info);
        wprintw(sub1, " If the information is correct then press 'y' ");
        wmove(a1, 15, 45);   // работа с доп. окном a1
        wprintw(a1, "%s", info);
        wrefresh(a1);
        ch = wgetch(sub1);
    } while (ch != 'y');
    return atoi(info);
}