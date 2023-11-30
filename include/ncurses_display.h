// macros
#ifndef NCURSES_DISPLAY_H
#define NCURSES_DISPLAY_H

#include <curses.h>

#include "process.h"
#include "system.h"

// defining a namespace for NCursesDisplay
// namespaces used to organize code into logical groups
namespace NCursesDisplay {
    void Display(System &system);
    void DisplaySystem(System &system, WINDOW *window);
    std::string ProgressBar(float percent);
};

#endif
