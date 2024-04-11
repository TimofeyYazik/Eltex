#ifndef FILE_MANAGER_STYLE_TERM_STYLE_H
#define FILE_MANAGER_STYLE_TERM_STYLE_H

#include <ncurses.h>

#include "../castom_type.h"

void SettingConsole(WINDOW **left_window, WINDOW **right_window, 
                    DirList *left_dir, DirList *right_dir, SwitchDir *sw_dr);
                    
void DrawButtons(DirList dir_button, int selected, WINDOW *window_name);

#endif //FILE_MANAGER_STYLE_TERM_STYLE_H