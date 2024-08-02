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
void print_bolt_nut_washer_size(WINDOW *d1, bolt info[], int number, int bolt_diam)
{
    for (int i = 0; i < number; i++)
    {
        if (info[i].bolt_name == bolt_diam)
        {
            wmove(d1, 4, 1);
            wprintw(d1, "%9.1f%11.1f%11d%13.1f%9.1f", info[i].washer_thickness, info[i].nut_height,
                    info[i].thread_length, info[i].thread_pitch, info[i].chamfer);
            wrefresh(d1);
        }
    }
}

// Проверка 1: резьба в детали (превышение 0.5t крайней к гайке детали)
// Проверка 2: резьба в шайбе (возможность закрутить гайку)
// Строки в доп. окне b1
int bolt_check_thread(WINDOW *b1, bolt info[], int number, const int *arr)
{
    double thread_result;
    for (int i = 0; i < number; i++)
    {
        if (info[i].bolt_name == arr[0])
        {
            thread_result = arr[4] * info[i].washer_thickness + arr[2] + arr[3] - arr[1] + info[i].thread_length;
            if (thread_result > 0) // резьба в крайней к гайке детали
            {
                wmove(b1, 2, 16);
                wprintw(b1, "Thread in detail %.1f", fabs(thread_result));
                wrefresh(b1);
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
                wrefresh(b1);
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
                wrefresh(b1);
            }
        }
    }
    return 0;
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
    wmove(c1, 3, 1);
    wprintw(c1, "%15s%20s%17s", "ThreadRequirement", "TighteningNut", "TipCheck");
    wrefresh(c1);
    if (res1_2 == 0 && res3 == 0)
    {
        //printf("\t%18s%18s%18s\n", "YES", "YES", "YES");
        wmove(c1, 4, 1);
        wprintw(c1, "%17s%20s%17s", "YES", "YES", "YES");
        wrefresh(c1);
    } else if (res1_2 == 0 && res3 == 1)
    {
        //printf("\t%18s%18s%18s\n", "YES", "YES", "NO");
        wmove(c1, 4, 1);
        wprintw(c1, "%17s%20s%17s", "YES", "YES", "NO");
        wrefresh(c1);
    } else if (res1_2 == 1 && res3 == 0)
    {
        //printf("\t%18s%18s%18s\n", "NO", "YES", "YES");
        wmove(c1, 4, 1);
        wprintw(c1, "%17s%20s%17s", "NO", "YES", "YES");
        wrefresh(c1);
    } else if (res1_2 == 1 && res3 == 1)
    {
        //printf("\t%18s%18s%18s\n", "NO", "YES", "NO");
        wmove(c1, 4, 1);
        wprintw(c1, "%17s%20s%17s", "NO", "YES", "NO");
        wrefresh(c1);
    } else if (res1_2 == 2 && res3 == 0)
    {
        //printf("\t%18s%18s%18s\n", "YES", "NO", "YES");
        wmove(c1, 4, 1);
        wprintw(c1, "%17s%20s%17s", "YES", "NO", "YES");
        wrefresh(c1);
    } else if (res1_2 == 2 && res3 == 1)
    {
        //printf("\t%18s%18s%18s\n", "YES", "NO", "NO");
        wmove(c1, 4, 1);
        wprintw(c1, "%17s%20s%17s", "YES", "NO", "NO");
        wrefresh(c1);
    }
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
        wmove(sub1, 1, 4);
        wprintw(sub1, "If the information is correct then press 'y', if incorrect press 'n' ");
        wmove(a1, 1, 45);   // работа с доп. окном a1
        wprintw(a1, "%s", info_diam);
        wrefresh(a1);
        print_bolt_nut_washer_size(d1, info, number, atoi(info_diam));
        ch = wgetch(sub1);
        if (ch == 'n')
        {
            delete_char(a1, 1, 45, 2);
            delete_char(d1, 4, 1, 55);
        }
    } while (ch != 'y');
    return atoi(info_diam);
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
            delete_char(a1, 2, 45, 3);
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
            delete_char(a1, 4, 45, 3);
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
            delete_char(a1, 5, 45, 3);
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
            delete_char(a1, 7, 45, 1);
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
            delete_char(a1, 8, 45, 1);
    } while (ch != 'y');
    return atoi(info);
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