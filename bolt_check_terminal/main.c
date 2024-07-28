#include <curses.h>

#define LMAX 8
#define CENTER 1
#define PINK 100
#define USER1 101
#define USER2 102
#define USER3 0 // фоновый цвет soft labels

int main(void)
{
    WINDOW *sub;
    char info[5];
    char label_text[LMAX][8] = {"HELP", "RESET", "CALC", "EXIT"};
    int ch;
    int label;

    slk_init(3);
    initscr();

    start_color();
    if (!can_change_color())
        addstr("This probably won't work...\n");
    init_color(PINK, 1000, 750, 750);
    init_color(USER1, 2550, 1280, 640);
    init_color(USER2, 760, 960, 70);
    init_color(USER3, 0, 0, 1000);
    slk_color(0);

    // soft labels
    for (label = 0; label < LMAX; label++)
        slk_set(label + 1, label_text[label], CENTER);
    slk_refresh();

    // создаем доп. окно №1
    sub = subwin(stdscr, LINES - 26, COLS - 2, 1, 1);
    if (sub == NULL)
    {
        endwin();
        puts("Unable to create subwindow");
        return (1);
    }

    // создание цветовых пар
    start_color();
    init_pair(1, COLOR_BLUE, COLOR_WHITE);
    init_pair(2, COLOR_WHITE, COLOR_BLUE);
    init_pair(3, COLOR_WHITE, COLOR_MAGENTA);
    init_pair(4, COLOR_BLACK, USER2);
    //init_pair(5, PINK, USER1);

    // цвет базового окна
    bkgd(COLOR_PAIR(1));
    box(stdscr, 0, 0);
    refresh();

    do
    {
        wclear(sub);
        wbkgd(sub, COLOR_PAIR(2));
        box(stdscr, 0, 0);
        wmove(sub, 0, 0);
        waddstr(sub, "Enter bolt diameter: ");
        wgetnstr(sub, info, 4);
        wmove(sub, 1, 0);
        wprintw(sub, "Bolt diameter is %s. If the information is correct then press 'y' ", info);
        ch = wgetch(sub);
    } while (ch != 'y');

    refresh();

    do
    {
        wclear(sub);
        wbkgd(sub, COLOR_PAIR(3));
        box(stdscr, 0, 0);
        wmove(sub, 0, 0);
        waddstr(sub, "Enter bolt length: ");
        wgetnstr(sub, info, 4);
        wmove(sub, 1, 0);
        wprintw(sub, "Bolt length is %s. If the information is correct then press 'y' ", info);
        ch = wgetch(sub);
    } while (ch != 'y');

    refresh();

    do
    {
        wclear(sub);
        wbkgd(sub, COLOR_PAIR(4));
        box(stdscr, 0, 0);
        wmove(sub, 0, 0);
        waddstr(sub, "Thickness of parts under the head: ");
        wgetnstr(sub, info, 4);
        wmove(sub, 1, 0);
        wprintw(sub, "Thickness of parts is %s. If the information is correct then press 'y' ", info);
        ch = wgetch(sub);
    } while (ch != 'y');

    refresh();

    getch();

    endwin();
    return (0);
}
