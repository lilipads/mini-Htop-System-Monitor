#include "ncurses_display.h"
#include "system.h"

// #include <iostream>
// #include "format.h"

int main() {
  System system;
  NCursesDisplay::Display(system);
}