// main file that will take care of displaying the ncurses results of the project
// providing some interface

#include "ncurses_display.h"
#include "system.h"

int main() {
    System system;
    NCursesDisplay::Display(system);
}