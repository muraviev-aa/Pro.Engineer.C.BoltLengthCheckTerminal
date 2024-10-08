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

// Массив под входные данные
int connect_package[6];

// Диаметр болта
int bolt_diam = 0;

// Перечень длин болтов из ГОСТ
int bolt_length[] = {22, 25, 28, 30, 32, 35, 38, 40, 45, 50, 55,
                     60, 65, 70, 75, 80, 85, 90, 95, 100, 105,
                     110, 115, 120, 125, 130, 140, 150, 160, 170,
                     180, 190, 200, 220, 240, 260, 280, 300};

int main(void)
{
    WINDOW *sub1, *a, *a1, *b, *b1, *c, *c1, *d, *d1;
    bolt *info = (bolt *) malloc(SIZE * sizeof(bolt));
    if (!info)
        printf("Error while allocating memory!\n");
    FILE *fptr;
    char file_name[] = "7798.csv";
    int count;   // количество строк в файле
    int ch;      // для работы с клавишами
    int result1_2, result3;
    int maxx, maxy, halfx, halfy;

    // Количество soft labels
    char label_text[LMAX][8] = {"HELP", "RESET", "ABOUT", "EXIT"};
    int label;

    slk_init(3);
    initscr();
    keypad(stdscr, TRUE);   // включили чтение клавиш
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

    // Soft labels
    for (label = 0; label < LMAX; label++)
        slk_set(label + 1, label_text[label], CENTER);
    slk_refresh();

    // Определяем размеры и положение доп. окон
    getmaxyx(stdscr, maxy, maxx);
    halfx = maxx >> 1;
    halfy = maxy >> 1;

    // Создаем доп. окна
    sub1 = subwin(stdscr, LINES - 26, COLS - 2, 1, 1);
    a = subwin(stdscr, halfy - 2, halfx - 1, 3, 1);
    a1 = subwin(stdscr, halfy - 5, halfx - 3, 4, 3);
    b = subwin(stdscr, halfy - 2, halfx - 1, 3, halfx);
    b1 = subwin(stdscr, halfy - 4, halfx - 5, 4, halfx + 2);
    c = subwin(stdscr, halfy - 2, halfx - 1, halfy + 1, halfx);
    c1 = subwin(stdscr, halfy - 2, halfx - 2, halfy + 2, halfx + 2);
    d = subwin(stdscr, halfy - 2, halfx - 1, halfy + 1, 1);
    d1 = subwin(stdscr, halfy - 5, halfx - 2, halfy + 2, 2);

    if (sub1 == NULL || a == NULL || a1 == NULL || b == NULL || b1 == NULL
        || c == NULL || c1 == NULL || d == NULL || d1 == NULL)
    {
        endwin();
        puts("Unable to create subwindow");
        return (1);
    }

    // Создание цветовых пар
    start_color();
    init_pair(1, COLOR_BLUE, COLOR_WHITE);     // базовое окно базовый белый
    init_pair(2, COLOR_WHITE, COLOR_BLUE);     // базовый синий
    init_pair(3, COLOR_BLUE, PINK);            // базовый розовый
    init_pair(4, COLOR_BLACK, USER2);          // базовый салатовый
    init_pair(5, COLOR_WHITE, USER4);          // базовый темно-синий
    init_pair(6, COLOR_BLUE, USER5);           // базовый toxic желтый
    init_pair(7, COLOR_BLUE, USER6);           // базовый оранжевый
    init_pair(8, COLOR_BLUE, USER7);           // базовый toxic желтый
    init_pair(9, COLOR_WHITE, COLOR_RED);      // файл не найден

    // Пишем в каждом доп. окне
    wbkgd(a, COLOR_PAIR(2));
    wbkgd(a1, COLOR_PAIR(2));
    wbkgd(b, COLOR_PAIR(2));
    wbkgd(b1, COLOR_PAIR(2));
    wbkgd(c, COLOR_PAIR(2));
    wbkgd(c1, COLOR_PAIR(2));
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
    wmove(d1, 3, 2);
    wprintw(d1, "%s%11s%11s%13s%9s", "WashThick", "NutHeight", "ThreadLen", "ThreadPitch", "Chamfer");

    wrefresh(a);
    wrefresh(a1);
    wrefresh(b);
    wrefresh(b1);
    wrefresh(c);
    wrefresh(c1);
    wrefresh(d);
    wrefresh(d1);

    // Базовое окно терминала
    bkgd(COLOR_PAIR(1));
    box(stdscr, 0, 0);
    refresh();

    /* Работа с доп. окном d) */
    // Работа с файлом
    open_file(b1, &fptr, file_name);
    count = read_data_file(&fptr, info);
    fclose(fptr);

    /* Работаем с горячими клавишами*/
    enter_hotkeys(sub1, 8);

    /* Работа с доп. окном a) */
    // 1. Вводим диаметр болта
    connect_package[0] = enter_data_bolt_diam(sub1, a1, d1, 8, info, count);
    refresh();

    // 2. Вводим длину болта
    connect_package[1] = enter_data_bolt_length(sub1, a1, d1, c1, 4, info, count);
    refresh();

    // 3. Вводим толщину деталей (детали) под головкой болта
    connect_package[2] = enter_data_thick_parts_head(sub1, a1, c1, 5);
    refresh();

    // 4. Вводим толщину детали под гайкой
    connect_package[3] = enter_data_thick_part_nut(sub1, a1, c1, 3);
    refresh();

    // 5. Вводим количество шайб под головкой болта
    connect_package[4] = enter_data_number_wash_head(sub1, a1, c1, 6);
    refresh();

    // 6. Вводим количество шайб под гайкой
    connect_package[5] = enter_data_number_wash_nut(sub1, a1, c1, 7);
    refresh();

    /* Работа с доп. окном b) */
    // Проверяем место нахождения резьбы
    result1_2 = bolt_check_thread(b1, c1, info, count, connect_package);
    refresh();

    // Проверяем длину кончика болта, выступающего из гайки
    result3 = bolt_tip_check(b1, info, count, connect_package);
    refresh();

    /* Работа с доп. окном c) */
    print_result_check(c1, result1_2, result3);
    refresh();

    // Освобождаем память
    free(info);

    /* Закрываем окно по нажатию клавиши F4 */
    if (getch() == KEY_F(4))
    {
        endwin();
        exit(0);
    }

    getch();
    endwin();
    return 0;
}


