#include <curses.h>
#include "temp_function.h"

#define LMAX 12
#define CENTER 1
#define PINK 100
#define USER1 101
#define USER2 102
#define USER3 0 // фоновый цвет soft labels
#define USER4 103



int main(void)
{
    WINDOW *sub1;
    char bolt_diam[] = "Enter bolt diameter: ";
    char bolt_diam_result[] = "Bolt diameter is %s.";
    char bolt_length[] = "2. Enter bolt length: ";
    char bolt_length_result[] = "Bolt length is %s.";
    char thick_parts_head[] = "3. Thickness of parts under the head: ";
    char thick_parts_head_result[] = "Thickness of parts under the head is %s.";
    char thick_part_nut[] = "4. Thickness of the part under the nut: ";
    char thick_part_nut_result[] = "Thickness of the part under the nut is %s.";

    char info[5];
    char label_text[LMAX][8] = {"HELP", "RESET", "CALC", "EXIT"};
    int ch;
    int label;

    slk_init(3);
    initscr();

    start_color();
    if (!can_change_color())
        addstr("This probably won't work...\n");

    init_color(PINK, 1000, 750, 750);      // использован
    init_color(USER1, 2550, 1280, 640);    // использован
    init_color(USER2, 760, 960, 70);       // использован
    init_color(USER3, 0, 0, 1000);         // фоновый цвет soft labels
    init_color(USER4, 200, 80, 690);       // использован
    slk_color(0);

    // soft labels
    for (label = 0; label < LMAX; label++)
        slk_set(label + 1, label_text[label], CENTER);
    slk_refresh();

    // создаем доп. окна
    sub1 = subwin(stdscr, LINES - 26, COLS - 2, 1, 1);
    if (sub1 == NULL)
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
    init_pair(5, COLOR_WHITE, USER4);          // базовый темно синий

    // базовое окно терминала
    bkgd(COLOR_PAIR(1));
    box(stdscr, 0, 0);
    refresh();

    // 1. Вводим диаметр болта
    enter_data(sub1, info, bolt_diam, bolt_diam_result, 2);
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

    getch();

    endwin();
    return 0;
}


