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

// Открыть файл
int open_file(FILE **fptr, char *name_file);

// Добавление записей
void add_record(bolt info[], int number, unsigned int bolt_name, double washer_thickness,
                double nut_height, unsigned int thread_length, double thread_pitch, double chamfer);

// Считываем данные из файла *.csv
int read_data_file(FILE **fptr, bolt info[]);

// Печатаем размеры болта, гайки и шайбы из ГОСТ
void print_bolt_nut_washer_size(WINDOW *d1, bolt info[], int number, int bolt_diam);

// Ввод диаметра болта
int enter_data_bolt_diam(WINDOW *sub1, WINDOW *a1, WINDOW *d1, int pair_num, bolt info[], int number);

// Ввод длины болта
int enter_data_bolt_length(WINDOW *sub1, WINDOW *a1, int pair_num);

// Ввод толщины соединяемых деталей (детали) под головкой болта
int enter_data_thick_parts_head(WINDOW *sub1, WINDOW *a1, int pair_num);

// Ввод толщины присоединяемой детали под гайкой
int enter_data_thick_part_nut(WINDOW *sub1, WINDOW *a1, int pair_num);

// Ввод количества шайб под головкой болта
int enter_data_number_wash_head(WINDOW *sub1, WINDOW *a1, int pair_num);

// Ввод количества шайб под гайкой
int enter_data_number_wash_nut(WINDOW *sub1, WINDOW *a1, int pair_num);

// Удаляем неверно введенные символы
void delete_char(WINDOW *w, int row, int column, int count_ch);

#endif //BOLT_CHECK_TERMINAL_TEMP_FUNCTION_H
