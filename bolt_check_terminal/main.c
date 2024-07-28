#include <curses.h>
#include "temp_function.h"

#define LMAX 12
#define CENTER 1
#define PINK 100
#define USER1 101
#define USER2 102
#define USER3 0 // фоновый цвет soft labels
#define USER4 103
#define USER5 104
#define USER6 105
#define USER7 106

int main(void)
{
    WINDOW *sub1, *a, *b, *c;
    int maxx, maxy, halfx, halfy;
    int bolt_diam_num = 0;               // диаметр болта
    int bolt_length_num = 0;             // длина болта
    int thick_parts_head_num = 0;        // толщина деталей (детали) под головкой болта
    int thick_part_nut_num = 0;          // толщина детали под гайкой
    int number_wash_head_num = 0;        // количество шайб под головкой болта
    int number_wash_nut_num = 0;         // количество шайб под гайками

    char bolt_diam[] = "1. Enter bolt diameter: ";
    char bolt_diam_result[] = "Bolt diameter is %s.";
    char bolt_length[] = "2. Enter bolt length: ";
    char bolt_length_result[] = "Bolt length is %s.";
    char thick_parts_head[] = "3. Thickness of parts under the head: ";
    char thick_parts_head_result[] = "Thickness of parts under the head is %s.";
    char thick_part_nut[] = "4. Thickness of the part under the nut: ";
    char thick_part_nut_result[] = "Thickness of the part under the nut is %s.";
    char number_wash_head[] = "5. Number of washers under the bolt head: ";
    char number_wash_head_result[] = "Number of washers under the bolt head is %s. ";
    char number_wash_nut[] = "6. Number of washers under nuts: ";
    char number_wash_nut_result[] = "Number of washers under nuts is %s. ";

    char info[5];
    char label_text[LMAX][8] = {"HELP", "RESET", "CALC", "EXIT"};
    int label;

    slk_init(3);
    initscr();
    refresh();

    start_color();
    if (!can_change_color())
        addstr("This probably won't work...\n");

    init_color(PINK, 1000, 750, 750);      // использован
    init_color(USER1, 2550, 1280, 640);    // использован
    init_color(USER2, 760, 960, 70);       // использован
    init_color(USER3, 0, 0, 1000);         // фоновый цвет soft labels
    init_color(USER4, 200, 80, 690);       // использован
    init_color(USER5, 840, 970, 690);      // использован
    init_color(USER6, 1000, 620, 0);       // использован
    init_color(USER7, 1000, 990, 0);       // использован
    slk_color(0);

    // soft labels
    for (label = 0; label < LMAX; label++)
        slk_set(label + 1, label_text[label], CENTER);
    slk_refresh();

    // определяем размеры и положение доп. окон
    getmaxyx(stdscr, maxy, maxx);
    halfx = maxx >> 1;
    halfy = maxy >> 1;

    // создаем доп. окна
    sub1 = subwin(stdscr, LINES - 26, COLS - 2, 1, 1);
    a = subwin(stdscr, 2 * halfy - 4, halfx - 1, 3, 1);
    b = subwin(stdscr, halfy - 2, halfx - 1, 3, halfx);
    c = subwin(stdscr, halfy - 2, halfx - 1, halfy + 1, halfx);
    if (sub1 == NULL || a == NULL || b == NULL || c == NULL)
    {
        endwin();
        puts("Unable to create subwindow");
        return (1);
    }

    // создание цветовых пар
    start_color();
    init_pair(1, COLOR_BLUE, COLOR_WHITE);     // базовое окно базовый белый
    init_pair(2, COLOR_WHITE, COLOR_BLUE);     // базовый синий
    init_pair(3, COLOR_BLUE, PINK);            // базовый розовый
    init_pair(4, COLOR_BLACK, USER2);          // базовый салатовый
    init_pair(5, COLOR_WHITE, USER4);          // базовый темно-синий
    init_pair(6, COLOR_BLUE, USER5);           // базовый toxic желтый
    init_pair(7, COLOR_BLUE, USER6);           // базовый оранжевый
    init_pair(8, COLOR_BLUE, USER7);           // базовый toxic желтый

    // Пишем в каждом доп. окне
    wbkgd(a, COLOR_PAIR(2));
    wbkgd(b, COLOR_PAIR(2));
    wbkgd(c, COLOR_PAIR(2));
    box(a, 0, 0);
    box(b, 0, 0);
    box(c, 0, 0);
    mvwaddstr(a, 0, 2, " Entered data ");
    mvwaddstr(b, 0, 2, " Thread location and bolt end length ");
    mvwaddstr(c, 0, 2, " Test ");

    wrefresh(a);
    wrefresh(b);
    wrefresh(c);

    // базовое окно терминала
    bkgd(COLOR_PAIR(1));
    box(stdscr, 0, 0);
    refresh();

    // 1. Вводим диаметр болта
    enter_data(sub1, info, bolt_diam, bolt_diam_result, 8); // был 2
    refresh();

    // 2. Вводим длину болта
    enter_data(sub1, info, bolt_length, bolt_length_result, 4);
    refresh();

    // 3. Вводим толщину деталей (детали) под головкой болта
    enter_data(sub1, info, thick_parts_head, thick_parts_head_result, 5);
    refresh();

    // 4. Вводим толщину детали под гайкой
    enter_data(sub1, info, thick_part_nut, thick_part_nut_result, 3);
    refresh();

    // 5. Вводим количество шайб под головкой болта
    enter_data(sub1, info, number_wash_head, number_wash_head_result, 6);
    refresh();

    // 6. Вводим количество шайб под гайкой
    enter_data(sub1, info, number_wash_nut, number_wash_nut_result, 7);
    refresh();

    getch();

    endwin();
    return 0;
}


