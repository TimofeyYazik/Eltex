#include "handler.h"

#include <dirent.h>
#include <ncurses.h>
#include <sys/stat.h>

#include "../driver_dir/driver.h"
#include "../handler_touch/handler.h"
#include "../style_term/style.h"
#include "../vim_connect/process.h"

void _SaveNcursesSettings() {
  def_prog_mode();
  refresh();
}

void _RestoreNcursesSettings() {
  endwin();
  reset_prog_mode();
  refresh();
}

void TouchHandler(DirList *left_dir, WINDOW **left_window,
                  WINDOW **right_window, DirList *right_dir, SwitchDir *sw_dr) {
  int selected_button = 0;
  WINDOW *use_window = *left_window;
  int driver = 0;
  int switch_window = 1;
  while (1) {
    if (switch_window == LEFT_WINDOW_ACTIVITY)
      DrawButtons(*left_dir, selected_button, use_window);  // Отрисовать кнопки
    if (switch_window == RIGHT_WINDOW_ACTIVITY)
      DrawButtons(*right_dir, selected_button, use_window);
    int key = getch();
    int len_namelist = 0;
    switch (key) {
      case KEY_UP:
        len_namelist =
            sw_dr->win == 0 ? left_dir->len_namelist : right_dir->len_namelist;
        selected_button = (selected_button - 1 + len_namelist) % len_namelist;
        break;
      case KEY_DOWN:
        len_namelist =
            sw_dr->win == 0 ? left_dir->len_namelist : right_dir->len_namelist;
        selected_button = (selected_button + 1) % len_namelist;
        break;
      case KEY_ETR:
        if (switch_window == LEFT_WINDOW_ACTIVITY) {
          driver = DriverDir(
              left_dir, left_dir->namelist_dir[selected_button]->d_name, sw_dr);
          if (driver == FILE_VIM_OPEN) {
            _SaveNcursesSettings();
            VimCall(left_dir->namelist_dir[selected_button]->d_name);
            sw_dr->dr[0] = NULL;
            sw_dr->dr[1] = NULL;
            _RestoreNcursesSettings();
          }
        }
        if (switch_window == RIGHT_WINDOW_ACTIVITY) {
          driver = DriverDir(right_dir,
                             right_dir->namelist_dir[selected_button]->d_name,
                             sw_dr);
          if (driver == FILE_VIM_OPEN) {
            _SaveNcursesSettings();
            VimCall(right_dir->namelist_dir[selected_button]->d_name);
            sw_dr->dr[0] = NULL;
            sw_dr->dr[1] = NULL;
            _RestoreNcursesSettings();
          }
        }
        refresh();
        break;
      case KEY_TAB:
        selected_button = 0;
        switch_window = -switch_window;
        if (switch_window == LEFT_WINDOW_ACTIVITY) {
          sw_dr->win = 0;
          use_window = *left_window;
          driver = DriverDir(
              left_dir, left_dir->namelist_dir[selected_button]->d_name, sw_dr);
        }
        if (switch_window == RIGHT_WINDOW_ACTIVITY) {
          sw_dr->win = 1;
          use_window = *right_window;
          driver = DriverDir(right_dir,
                             right_dir->namelist_dir[selected_button]->d_name,
                             sw_dr);
        }
        break;
      case 'q':
        driver = -1;
        break;
    }
    if (driver == -1) {
      break;
    }
  }
}