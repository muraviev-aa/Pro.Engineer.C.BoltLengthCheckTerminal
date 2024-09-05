#include "temp_function.h"
#include <math.h>

int open_file(FILE **fptr, char *name_file)
{
    if ((*fptr = fopen(name_file, "r")) == NULL)
    {
        perror("Error opening file");
        return 1;
    }
    return 0;
}

void add_record(bolt info[], int number, unsigned int bolt_name, double washer_thickness,
                double nut_height, unsigned int thread_length, double thread_pitch, double chamfer)
{
    info[number].bolt_name = bolt_name;
    info[number].washer_thickness = washer_thickness;
    info[number].nut_height = nut_height;
    info[number].thread_length = thread_length;
    info[number].thread_pitch = thread_pitch;
    info[number].chamfer = chamfer;
}

int read_data_file(FILE **fptr, bolt info[])
{
    unsigned int bolt_name, thread_length;
    double washer_thickness, nut_height, thread_pitch, chamfer;
    int count = 0;
    while (fscanf(*fptr, "%d;%lf;%lf;%d;%lf;%lf", &bolt_name, &washer_thickness, &nut_height,
                  &thread_length, &thread_pitch, &chamfer) > 0)
    {
        add_record(info, count, bolt_name, washer_thickness, nut_height, thread_length, thread_pitch, chamfer);
        count++;
    }
    return count;
}

// Данные из ГОСТ в доп. окне d1
void print_bolt_nut_washer_size(WINDOW *d1, bolt info[], int number, int bolt_d, int b_length)
{
    int t_length = check_thread_length(bolt_d, b_length); // уточняем длину резьбы
    for (int i = 0; i < number; i++)
    {
        if (info[i].bolt_name == bolt_d)
        {
            int local_t_length = getInt(info, i, t_length);
            wmove(d1, 4, 1);
            wprintw(d1, "%10.1f%11.1f%11d%13.1f%9.1f", info[i].washer_thickness, info[i].nut_height,
                    local_t_length, info[i].thread_pitch, info[i].chamfer);
            wrefresh(d1);
        }
    }
}

// Уточнение по диаметру болта длины резьбы в зависимости от его длины
// only M10, M12, M16, M20, M24, M30
int check_thread_length(int bolt_d, int b_length)
{
    int thread_length = 0;
    if (bolt_d == 10 && b_length >= 130)
        thread_length = 32;
    else if (bolt_d == 12 && b_length >= 130 && b_length < 220)
        thread_length = 36;
    else if (bolt_d == 12 && b_length >= 220)
        thread_length = 49;
    else if (bolt_d == 16 && b_length >= 130 && b_length < 220)
        thread_length = 44;
    else if (bolt_d == 16 && b_length >= 220)
        thread_length = 57;
    else if (bolt_d == 20 && b_length >= 130 && b_length < 220)
        thread_length = 52;
    else if (bolt_d == 20 && b_length >= 220)
        thread_length = 65;
    else if (bolt_d == 24 && b_length >= 130 && b_length < 220)
        thread_length = 60;
    else if (bolt_d == 24 && b_length >= 220)
        thread_length = 73;
    else if (bolt_d == 30 && b_length >= 130 && b_length < 220)
        thread_length = 72;
    else if (bolt_d == 30 && b_length >= 220)
        thread_length = 85;
    return thread_length;
}

// Проверка 1: резьба в детали (превышение 0.5t крайней к гайке детали)
// Проверка 2: резьба в шайбе (возможность закрутить гайку)
// Строки в доп. окне b1
int bolt_check_thread(WINDOW *b1, WINDOW *c1, bolt info[], int number, const int *arr)
{
    double thread_result;
    for (int i = 0; i < number; i++)
    {
        int bolt_d = info[i].bolt_name;
        int b_length = arr[1];
        if (bolt_d == arr[0])
        {
            int t_length = check_thread_length(bolt_d, b_length); // уточняем длину резьбы
            int local_t_length = getInt(info, i, t_length);
            thread_result = arr[4] * info[i].washer_thickness + arr[2] + arr[3] - b_length + local_t_length;
            if (thread_result > 0) // резьба в крайней к гайке детали
            {
                wmove(b1, 2, 16);
                wprintw(b1, "Thread in detail %.1f", fabs(thread_result));
                wmove(c1, 3, 28);
                waddch(c1, ACS_DIAMOND); // знак резьбы
                wrefresh(b1);
                wrefresh(c1);
                if (thread_result > 0.5 * arr[3])
                {
                    wmove(b1, 3, 9);
                    wprintw(b1, "!!! The thread goes into the part !!!");
                    wrefresh(b1);
                    return 1;
                }
            } else if (thread_result < 0) // резьба в шайбе
            {
                wmove(b1, 2, 16);
                wprintw(b1, "Thread in washer %.1f ", fabs(thread_result));
                wmove(c1, 3, 31);
                waddch(c1, ACS_DIAMOND); // знак резьбы
                wrefresh(b1);
                wrefresh(c1);
                if (fabs(thread_result) > arr[5] * info[i].washer_thickness)
                {
                    printf("!!! Do not tighten the nut !!!");
                    wmove(b1, 3, 12);
                    wprintw(b1, "!!! Do not tighten the nut !!!");
                    wrefresh(b1);
                    return 2;
                }
            } else if (thread_result == 0) // резьба на границе деталей
            {
                wmove(b1, 2, 0);
                wprintw(b1, "Thread at the interface between the part and the washer");
                wmove(c1, 3, 29);
                waddch(c1, ACS_DIAMOND); // знак резьбы
                wmove(c1, 3, 28);
                waddch(c1, ACS_DIAMOND); // знак резьбы
                wrefresh(b1);
                wrefresh(c1);
            }
        }
    }
    return 0;
}

// Получение длины резьбы после проверки
int getInt(const bolt *info, int i, int t_length)
{
    int local_t_length;
    if (t_length == 0)
        local_t_length = info[i].thread_length;
    else
        local_t_length = t_length;
    return local_t_length;
}

// Проверка 3: проверка длины конца болта (не менее одного полного витка резьбы + фаска)
int bolt_tip_check(WINDOW *b1, bolt info[], int number, const int *arr)
{
    double bolt_tip;
    for (int i = 0; i < number; i++)
    {
        if (info[i].bolt_name == arr[0])
        {
            bolt_tip = arr[1] - info[i].washer_thickness * arr[4] - arr[2] - arr[3] -
                       info[i].washer_thickness * arr[5] - 2 * info[i].nut_height;
            wmove(b1, 5, 19);
            wprintw(b1, "Bolt tip is %.1f", bolt_tip);
            wrefresh(b1);
            if (bolt_tip < info[i].thread_pitch + info[i].chamfer)
            {
                wmove(b1, 6, 16);
                wprintw(b1, "!!! Short bolt tip !!!");
                wrefresh(b1);
                return 1;
            }
        }
    }
    return 0;
}

// Вывод результатов проверки
void print_result_check(WINDOW *c1, int res1_2, int res3)
{
    wmove(c1, 7, 1);
    wprintw(c1, "%15s%20s%17s", "ThreadRequirement", "TighteningNut", "TipCheck");
    wrefresh(c1);
    if (res1_2 == 0 && res3 == 0)
    {
        wmove(c1, 8, 1);
        wprintw(c1, "%17s%20s%17s", "YES", "YES", "YES");
        wrefresh(c1);
    } else if (res1_2 == 0 && res3 == 1)
    {
        wmove(c1, 8, 1);
        wprintw(c1, "%17s%20s%17s", "YES", "YES", "NO");
        wrefresh(c1);
    } else if (res1_2 == 1 && res3 == 0)
    {
        wmove(c1, 8, 1);
        wprintw(c1, "%17s%20s%17s", "NO", "YES", "YES");
        wrefresh(c1);
    } else if (res1_2 == 1 && res3 == 1)
    {
        wmove(c1, 8, 1);
        wprintw(c1, "%17s%20s%17s", "NO", "YES", "NO");
        wrefresh(c1);
    } else if (res1_2 == 2 && res3 == 0)
    {
        wmove(c1, 8, 1);
        wprintw(c1, "%17s%20s%17s", "YES", "NO", "YES");
        wrefresh(c1);
    } else if (res1_2 == 2 && res3 == 1)
    {
        wmove(c1, 8, 1);
        wprintw(c1, "%17s%20s%17s", "YES", "NO", "NO");
        wrefresh(c1);
    }
}

void enter_hotkeys(WINDOW *sub1, int pair_num)
{
    int ch;
    do
    {
        ch = wgetch(sub1);
        wclear(sub1);
        wbkgd(sub1, COLOR_PAIR(pair_num));
        box(stdscr, 0, 0);
        wmove(sub1, 0, 1);
        waddstr(sub1,
                "Enter hotkeys: <F1> - HELP; <F2> - RESET; <F3> - ABOUT; <F4> - EXIT or press Enter to continue ");
        wrefresh(sub1);
        switch (ch)
        {
            case KEY_F(1):
                // in the pipeline
                wmove(sub1, 1, 1);
                waddstr(sub1, "To work, you can use the hot keys F1, F2, F3, F4");
                break;
            case KEY_F(2):
                // in the pipeline
                wmove(sub1, 1, 1);
                waddstr(sub1, "<F2> - RESET");
                break;
            case KEY_F(3):
                wmove(sub1, 1, 1);
                waddstr(sub1, "This is a program for checking bolt length in building structures. "
                              "Developer Muraviev A.A. Free software licence.");
                break;
            case KEY_F(4):
                endwin();
                exit(0);
            default:
                break;
        }
        refresh();
    } while (ch != '\n');
}

int enter_data_bolt_diam(WINDOW *sub1, WINDOW *a1, WINDOW *d1, int pair_num, bolt info[], int number)
{
    int ch;
    char info_diam[3];
    do
    {
        wclear(sub1);
        wbkgd(sub1, COLOR_PAIR(pair_num));
        box(stdscr, 0, 0);
        wmove(sub1, 0, 1);
        waddstr(sub1, "1. Enter bolt diameter: ");
        wgetnstr(sub1, info_diam, 2);
        bolt_diam = atoi(info_diam);
        wmove(sub1, 1, 4);
        if (bolt_diam != 6 && bolt_diam != 8 && bolt_diam != 10 && bolt_diam != 12 && bolt_diam != 16
            && bolt_diam != 20 && bolt_diam != 24 && bolt_diam != 30)
        {
            if (bolt_diam == 14 || bolt_diam == 18 || bolt_diam == 22 || bolt_diam == 27)
                wprintw(sub1, "!!! Not recommended diameter !!! ");
            else if (bolt_diam == 36 || bolt_diam == 42 || bolt_diam == 48)
                wprintw(sub1, "!!! Unsupported diameter !!! ");
            else
                wprintw(sub1, "!!! Incorrect diameter !!! ");
        }
        wprintw(sub1, "If the information is correct then press 'y', if incorrect press 'n' ");
        wmove(a1, 1, 45);   // работа с доп. окном a1
        wprintw(a1, "%s", info_diam);
        wrefresh(a1);
        //print_bolt_nut_washer_size(d1, info, number, bolt_diam);
        ch = wgetch(sub1);
        if (ch == 'n')
        {
            delete_char(a1, 1, 45, 2);
            delete_char(d1, 4, 1, 55);
        }
    } while (ch != 'y');
    return bolt_diam;
}

int enter_data_bolt_length(WINDOW *sub1, WINDOW *a1, WINDOW *d1, WINDOW *c1, int pair_num, bolt info[], int number)
{
    int ch, flag = 0;
    char local_info[4];
    int b_length;
    do
    {
        wclear(sub1);
        wbkgd(sub1, COLOR_PAIR(pair_num));
        box(stdscr, 0, 0);
        wmove(sub1, 0, 1);
        waddstr(sub1, "2. Enter bolt length: ");
        wgetnstr(sub1, local_info, 3);
        wmove(sub1, 1, 4);
        for (int i = 0; i < 38; i++)
        {
            if (bolt_length[i] == atoi(local_info))
                flag++;
        }
        if (flag == 0)
            wprintw(sub1, "!!! Incorrect length !!! ");
        wprintw(sub1, "If the information is correct then press 'y', if incorrect press 'n' ");
        wmove(a1, 2, 45);   // работа с доп. окном a1
        wprintw(a1, "%s", local_info);
        b_length = atoi(local_info);
        wrefresh(a1);
        print_bolt_nut_washer_size(d1, info, number, bolt_diam, b_length); // печать данных их файла в доп. окно d1
        ch = wgetch(sub1);
        if (ch == 'n')
        {
            delete_char(a1, 2, 45, 3);
            ch = 0;
        }
    } while (ch != 'y');
    /* Рисуем болт */
    // головка болта
    for (int j = 2; j <= 4; j++)
    {
        wmove(c1, j, 18);
        waddch(c1, ACS_BLOCK);
    }
    // тело болта
    for (int i = 19; i <= 33; i++)
    {
        wmove(c1, 3, i);
        waddch(c1, ACS_LANTERN);
    }
    /* Рисуем первую гайку */
    for (int j = 2; j <= 4; j++)
    {
        wmove(c1, j, 34);
        waddch(c1, ACS_BLOCK);
    }
    wmove(c1, 3, 35);
    waddch(c1, ACS_LANTERN);
    /* Рисуем вторую гайку */
    for (int j = 2; j <= 4; j++)
    {
        wmove(c1, j, 36);
        waddch(c1, ACS_BLOCK);
    }
    // конец болта
    wmove(c1, 3, 37);
    waddch(c1, ACS_LANTERN);
    return b_length;
}

int enter_data_thick_parts_head(WINDOW *sub1, WINDOW *a1, WINDOW *c1, int pair_num)
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
            delete_char(a1, 4, 45, 3);
    } while (ch != 'y');
    /* Рисуем деталь под головкой болта */
    for (int i = 24; i <= 25; i++)
    {
        wmove(c1, 1, i);
        waddch(c1, ACS_BOARD);
        wmove(c1, 2, i);
        waddch(c1, ACS_BOARD);
        wmove(c1, 4, i);
        waddch(c1, ACS_BOARD);
        wmove(c1, 5, i);
        waddch(c1, ACS_BOARD);
    }

    return atoi(info);
}

int enter_data_thick_part_nut(WINDOW *sub1, WINDOW *a1, WINDOW *c1, int pair_num)
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
            delete_char(a1, 5, 45, 3);
    } while (ch != 'y');
    /* Рисуем деталь под гайкой болта */
    for (int i = 27; i <= 28; i++)
    {
        wmove(c1, 1, i);
        waddch(c1, ACS_CKBOARD);
        wmove(c1, 2, i);
        waddch(c1, ACS_CKBOARD);
        wmove(c1, 4, i);
        waddch(c1, ACS_CKBOARD);
        wmove(c1, 5, i);
        waddch(c1, ACS_CKBOARD);
    }
    return atoi(info);
}

int enter_data_number_wash_head(WINDOW *sub1, WINDOW *a1, WINDOW *c1, int pair_num)
{
    int ch;
    int result_wash;
    char info[2];
    do
    {
        wclear(sub1);
        wbkgd(sub1, COLOR_PAIR(pair_num));
        box(stdscr, 0, 0);
        wmove(sub1, 0, 1);
        waddstr(sub1, "5. Number of washers under the bolt head: ");
        wgetnstr(sub1, info, 1);
        result_wash = atoi(info);
        wmove(sub1, 1, 4);
        wprintw(sub1, "If the information is correct then press 'y', if incorrect press 'n' ");
        wmove(a1, 7, 45);   // работа с доп. окном a1
        wprintw(a1, "%d", result_wash);
        wrefresh(a1);
        ch = wgetch(sub1);
        if (ch == 'n')
            delete_char(a1, 7, 45, 1);
    } while (ch != 'y');
    /* Рисуем шайбу под головкой болта */
    if (result_wash != 0)
    {
        wmove(c1, 2, 21);
        waddch(c1, ACS_VLINE);
        wmove(c1, 4, 21);
        waddch(c1, ACS_VLINE);
    }
    return result_wash;
}

int enter_data_number_wash_nut(WINDOW *sub1, WINDOW *a1, WINDOW *c1, int pair_num)
{
    int ch;
    int result_wash;
    char info[2];
    do
    {
        wclear(sub1);
        wbkgd(sub1, COLOR_PAIR(pair_num));
        box(stdscr, 0, 0);
        wmove(sub1, 0, 1);
        waddstr(sub1, "6. Number of washers under nuts: ");
        wgetnstr(sub1, info, 3);
        result_wash = atoi(info);
        wmove(sub1, 1, 4);
        wprintw(sub1, "If the information is correct then press 'y', if incorrect press 'n' ");
        wmove(a1, 8, 45);   // работа с доп. окном a1
        wprintw(a1, "%d", result_wash);
        wrefresh(a1);
        ch = wgetch(sub1);
        if (ch == 'n')
            delete_char(a1, 8, 45, 1);
    } while (ch != 'y');
    /* Рисуем шайбы под гайкой */
    if (result_wash != 0)
    {
        wmove(c1, 2, 31);
        waddch(c1, ACS_VLINE);
        wmove(c1, 4, 31);
        waddch(c1, ACS_VLINE);
        if (result_wash == 2)
        {
            wmove(c1, 2, 32);
            waddch(c1, ACS_VLINE);
            wmove(c1, 4, 32);
            waddch(c1, ACS_VLINE);
        }
    }
    return result_wash;
}

void delete_char(WINDOW *w, int row, int column, int count_ch)
{
    for (int i = 0; i < count_ch; i++)
    {
        wmove(w, row, column++);
        waddrawch(w, ' ');
        wrefresh(w);
    }
}