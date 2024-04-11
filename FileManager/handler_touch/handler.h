#ifndef FILE_MANAGER_HANDLER_TOUCH_HANDLER_H_
#define FILE_MANAGER_HANDLER_TOUCH_HANDLER_H_

#include "../castom_type.h"
#include <ncurses.h>

void TouchHandler( DirList *left_dir, WINDOW **left_window, WINDOW **right_window,
                  DirList *right_dir, SwitchDir *sw_dr);

#endif // FILE_MANAGER_HANDLER_TOUCH_HANDLER_H_