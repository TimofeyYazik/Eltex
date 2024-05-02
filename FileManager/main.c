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
  if(sw_dr.dr[0] != NULL)
    closedir(sw_dr.dr[0]);
  if(sw_dr.dr[1] != NULL)
    closedir(sw_dr.dr[1]);
  delwin(right_window);
  delwin(left_window);
  for(int i = 0; i < left_dir.len_namelist; i++){
    free(left_dir.namelist_dir[i]);
  }
  free(left_dir.namelist_dir);
  for(int i = 0; i < right_dir.len_namelist; i++){
    free(right_dir.namelist_dir[i]);
  }
  free(right_dir.namelist_dir);
  endwin();
  exit(EXIT_SUCCESS);
}