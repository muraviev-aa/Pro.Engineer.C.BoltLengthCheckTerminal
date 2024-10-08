#ifndef BOLT_CHECK_TERMINAL_TEMP_FUNCTION_H
#define BOLT_CHECK_TERMINAL_TEMP_FUNCTION_H

#include <curses.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct
{
    unsigned int bolt_name;       // диаметр болта
    double washer_thickness;      // толщина шайбы
    double nut_height;            // высота гайки
    unsigned int thread_length;   // длина резьбы
    double thread_pitch;          // шаг резьбы
    double chamfer;               // фаска
} bolt;

/* Массив под входные данные
 * connect_package[0] - диаметр болта
 * connect_package[1] - длина болта
 * connect_package[2] - толщина детали (деталей) под головкой болта
 * connect_package[3] - толщина детали под гайкой
 * connect_package[4] - количество шайб под головкой болта
 * connect_package[5] - количество шайб под гайкой
*/
extern int connect_package[6];

// Диаметр болта
extern int bolt_diam;

// Массив под длины болтов
extern int bolt_length[38];

// Открыть файл
int open_file(WINDOW *b1, FILE **fptr, char *name_file);

// Добавление записей
void add_record(bolt info[], int number, unsigned int bolt_name, double washer_thickness,
                double nut_height, unsigned int thread_length, double thread_pitch, double chamfer);

// Считываем данные из файла *.csv
int read_data_file(FILE **fptr, bolt info[]);

// Печатаем размеры болта, гайки и шайбы из ГОСТ
void print_bolt_nut_washer_size(WINDOW *d1, bolt info[], int number, int bolt_diam, int b_length);

// Уточнение по диаметру болта длины резьбы в зависимости от его длины
int check_thread_length(int bolt_d, int b_length);

// Получение длины резьбы после проверки
int getInt(const bolt *info, int i, int t_length);

// Работаем с горячими клавишами
void enter_hotkeys(WINDOW *sub1, int pair_num);

// Ввод диаметра болта
int enter_data_bolt_diam(WINDOW *sub1, WINDOW *a1, WINDOW *d1, int pair_num, bolt info[], int number);

// Ввод длины болта
int enter_data_bolt_length(WINDOW *sub1, WINDOW *a1, WINDOW *d1, WINDOW *c1, int pair_num, bolt info1[], int number);

// Ввод толщины соединяемых деталей (детали) под головкой болта
int enter_data_thick_parts_head(WINDOW *sub1, WINDOW *a1, WINDOW *c1, int pair_num);

// Ввод толщины присоединяемой детали под гайкой
int enter_data_thick_part_nut(WINDOW *sub1, WINDOW *a1, WINDOW *c1, int pair_num);

// Ввод количества шайб под головкой болта
int enter_data_number_wash_head(WINDOW *sub1, WINDOW *a1, WINDOW *c1, int pair_num);

// Ввод количества шайб под гайкой
int enter_data_number_wash_nut(WINDOW *sub1, WINDOW *a1, WINDOW *c1, int pair_num);

// Удаляем символы
void delete_char(WINDOW *w, int row, int column, int count_ch);

// Определяем расположение резьбы
int bolt_check_thread(WINDOW *b1, WINDOW *c1, bolt info[], int number, const int *arr);

// Определяем длину кончика болта, выступающего из гайки
int bolt_tip_check(WINDOW *b1, bolt info[], int number, const int *arr);

// Вывод результатов проверки
void print_result_check(WINDOW *c1, int res1_2, int res3);

#endif //BOLT_CHECK_TERMINAL_TEMP_FUNCTION_H
