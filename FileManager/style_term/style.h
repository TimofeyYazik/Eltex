#ifndef FILE_MANAGER_STYLE_TERM_STYLE_H
#define FILE_MANAGER_STYLE_TERM_STYLE_H

#include <ncurses.h>

void draw_buttons(struct dirent **namelist_dir, int count, int selected, WINDOW *window_name);

#endif //FILE_MANAGER_STYLE_TERM_STYLE_H