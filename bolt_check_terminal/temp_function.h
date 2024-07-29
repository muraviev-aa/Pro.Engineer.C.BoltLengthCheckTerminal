#ifndef BOLT_CHECK_TERMINAL_TEMP_FUNCTION_H
#define BOLT_CHECK_TERMINAL_TEMP_FUNCTION_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>

// Ввод диаметра болта
int enter_data_bolt_diam(WINDOW *sub1, WINDOW *a1, int pair_num);

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

#endif //BOLT_CHECK_TERMINAL_TEMP_FUNCTION_H
