#ifndef BOLT_CHECK_TERMINAL_TEMP_FUNCTION_H
#define BOLT_CHECK_TERMINAL_TEMP_FUNCTION_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>

// ввод исходных данных болтового соединения
void enter_data(WINDOW *sub1, char *info, char *arr, char *arr1,  int pair_num);

#endif //BOLT_CHECK_TERMINAL_TEMP_FUNCTION_H
