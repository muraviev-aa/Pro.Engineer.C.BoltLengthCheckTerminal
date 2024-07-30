#include <curses.h>
#include "temp_function.h"

#define LMAX 12
#define CENTER 1
#define PINK 100
#define USER1 101
#define USER2 102
#define USER3 0     // фоновый цвет soft labels
#define USER4 103
#define USER5 104
#define USER6 105
#define USER7 106
#define SIZE 8      // число строк в файле

int connect_package[6];

int main(void)
{
    WINDOW *sub1, *a, *a1, *b, *b1, *c, *d, *d1;
    bolt *info = (bolt *) malloc(SIZE * sizeof(bolt));
    if (!info)
        printf("Error while allocating memory!\n");
    FILE *fptr;
    char file_name[] = "7798.csv";
    int count;   // количество строк в файле
    int result1_2, result3;
    int maxx, maxy, halfx, halfy;
    int bolt_diam = 0;                  // диаметр болта
    int bolt_length = 0;                // длина болта
    int thick_parts_head = 0;           // толщина деталей (детали) под головкой болта
    int thick_part_nut = 0;             // толщина детали под гайкой
    int number_wash_head = 0;           // количество шайб под головкой болта
    int number_wash_nut = 0;            // количество шайб под гайками

    // Количество soft labels
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
    a = subwin(stdscr, halfy - 2, halfx - 1, 3, 1);
    a1 = subwin(stdscr, halfy - 5, halfx - 2, 4, 3);
    b = subwin(stdscr, halfy - 2, halfx - 1, 3, halfx);
    b1 = subwin(stdscr, halfy - 5, halfx - 2, 4, halfx + 2);
    c = subwin(stdscr, halfy - 2, halfx - 1, halfy + 1, halfx);
    d = subwin(stdscr, halfy - 2, halfx - 1, halfy + 1, 1);
    d1 = subwin(stdscr, halfy - 5, halfx - 2, halfy + 2, 3);

    if (sub1 == NULL || a == NULL || a1 == NULL || b == NULL || b1 == NULL
        || c == NULL || d == NULL || d1 == NULL)
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
    wbkgd(a1, COLOR_PAIR(2));
    wbkgd(b, COLOR_PAIR(2));
    wbkgd(b1, COLOR_PAIR(2));
    wbkgd(c, COLOR_PAIR(2));
    wbkgd(d, COLOR_PAIR(2));
    wbkgd(d1, COLOR_PAIR(2));
    box(a, 0, 0);
    box(b, 0, 0);
    box(c, 0, 0);
    box(d, 0, 0);
    mvwaddstr(a, 0, 1, " a) Entered data ");
    mvwaddstr(a1, 1, 5, "BOLT DIAMETER");
    mvwaddstr(a1, 2, 5, "BOLT LENGTH");
    mvwaddstr(a1, 4, 5, "THICKNESS OF PARTS UNDER THE HEAD");
    mvwaddstr(a1, 5, 5, "THICKNESS OF THE PARTS UNDER THE NUT");
    mvwaddstr(a1, 7, 5, "NUMBER OF WASHERS UNDER THE BOLT HEAD");
    mvwaddstr(a1, 8, 5, "NUMBER OF WASHERS UNDER NUTS");
    mvwaddstr(b, 0, 1, " b) Thread location and bolt end length ");
    mvwaddstr(c, 0, 1, " c) Result ");
    mvwaddstr(d, 0, 1, " d) GOST 7798-70, GOST 11371-78, GOST 5915-70 ");
    wmove(d1, 3, 1);
    wprintw(d1, "%s%11s%11s%13s%9s", "WashThick", "NutHeight", "ThreadLen", "ThreadPitch", "Chamfer");
    /*wmove(b1, 1, 1);
    wprintw(b1, "TEST");*/

    wrefresh(a);
    wrefresh(a1);
    wrefresh(b);
    wrefresh(b1);
    wrefresh(c);
    wrefresh(d);
    wrefresh(d1);

    // базовое окно терминала
    bkgd(COLOR_PAIR(1));
    box(stdscr, 0, 0);
    refresh();

    // Работа с файлом
    open_file(&fptr, file_name);
    count = read_data_file(&fptr, info);
    fclose(fptr);

    // 1. Вводим диаметр болта
    connect_package[0] = enter_data_bolt_diam(sub1, a1, d1, 8, info, count);
    refresh();

    // 2. Вводим длину болта
    connect_package[1] = enter_data_bolt_length(sub1, a1, 4);
    refresh();

    // 3. Вводим толщину деталей (детали) под головкой болта
    connect_package[2] = enter_data_thick_parts_head(sub1, a1, 5);
    refresh();

    // 4. Вводим толщину детали под гайкой
    connect_package[3] = enter_data_thick_part_nut(sub1, a1, 3);
    refresh();

    // 5. Вводим количество шайб под головкой болта
    connect_package[4] = enter_data_number_wash_head(sub1, a1, 6);
    refresh();

    // 6. Вводим количество шайб под гайкой
    connect_package[5] = enter_data_number_wash_nut(sub1, a1, 7);
    refresh();

    // Проверяем место нахождения резьбы
    result1_2 = bolt_check_thread(b1, info, count, connect_package);
    refresh();

    // Проверяем длину кончика болта, выступающего из гайки
    result3 = bolt_tip_check(b1, info, count, connect_package);
    refresh();


    getch();

    free(info);
    endwin();
    return 0;
}


