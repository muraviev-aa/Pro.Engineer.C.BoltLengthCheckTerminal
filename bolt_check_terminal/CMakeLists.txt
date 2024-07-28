cmake_minimum_required(VERSION 3.28)
project(bolt_check_terminal C)

set(CMAKE_C_STANDARD 11)
set(CURSES_INCLUDE_PATH "C:/tools/PDCurses-3.9")
set(CURSES_LIBRARY "C:/tools/PDCurses-3.9/wincon/pdcurses.a")

find_package( Curses REQUIRED )
include_directories( ${CURSES_INCLUDE_DIR} )

add_executable(bolt_check_terminal main.c)

target_link_libraries( bolt_check_terminal ${CURSES_LIBRARIES} )
