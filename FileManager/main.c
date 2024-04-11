
#include <ncurses.h>
#include <stdlib.h>

#include "castom_type.h"
#include "driver_dir/driver.h"
#include "handler_touch/handler.h"
#include "style_term/style.h"
#include "vim_connect/process.h"

int main() {
  SwitchDir sw_dr = {0};
  DirList left_dir = {0};
  DirList right_dir = {0};
  initscr();
  start_color();
  WINDOW *left_window = NULL;
  WINDOW *right_window = NULL;
  SettingConsole(&left_window, &right_window, &left_dir, &right_dir, &sw_dr);
  DrawButtons(right_dir, 0, right_window);
  TouchHandler(&left_dir, &left_window, &right_window, &right_dir, &sw_dr);
  closedir(sw_dr.dr[0]);
  closedir(sw_dr.dr[1]);
  delwin(right_window);
  delwin(left_window);
  endwin();
  exit(EXIT_SUCCESS);
}
