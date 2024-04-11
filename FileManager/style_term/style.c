#include "style.h"

#include <dirent.h>
#include <ncurses.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "../driver_dir/driver.h"

void SettingConsole(WINDOW **left_window, WINDOW **right_window,
                    DirList *left_dir, DirList *right_dir, SwitchDir *sw_dr) {
  int driver = DriverDir(left_dir, ".", sw_dr);
  sw_dr->win = 1;
  driver = DriverDir(right_dir, ".", sw_dr);
  if (driver == -1) {
    exit(EXIT_FAILURE);
  }
  sw_dr->win = 0;
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(FALSE);
  refresh();
  int max_y, max_x;
  getmaxyx(stdscr, max_y, max_x);
  int start_x = max_x / 2;
  int width = max_x - start_x;
  *left_window = newwin(max_y, width, 0, 0);
  *right_window = newwin(max_y, width, 0, start_x);
}

void DrawButtons(DirList dir_button, int selected, WINDOW *window_name) {
  init_pair(1, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(2, COLOR_CYAN, COLOR_BLACK);
  struct stat statbuf;
  werase(window_name);
  int start = 0;
  int max_y, max_x;
  getmaxyx(window_name, max_y, max_x);
  box(window_name, '|', '-');
  if (selected > max_y - 2) {
    start = selected - 5;
  }
  for (int i = start, l = 0; i < dir_button.len_namelist; ++i, ++l) {
    stat(dir_button.namelist_dir[i]->d_name, &statbuf);
    if (i == selected) {
      if (S_ISREG(statbuf.st_mode))
        wattron(window_name, COLOR_PAIR(1) | A_REVERSE);
      else
        wattron(window_name, COLOR_PAIR(2) | A_REVERSE);
    }
    mvwprintw(window_name, l + 1, 1, "%s", dir_button.namelist_dir[i]->d_name);
    if (i == selected) {
      if (S_ISREG(statbuf.st_mode))
        wattroff(window_name, COLOR_PAIR(1) | A_REVERSE);
      else
        wattroff(window_name, COLOR_PAIR(2) | A_REVERSE);
    }
  }
  wrefresh(window_name);
}