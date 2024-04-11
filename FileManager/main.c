#include <signal.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <ncurses.h>
#include <string.h>


#include "driver_dir/driver.h"
#include "style_term/style.h"

#define TRUE 1
#define FALSE 0

void sig_winch(int signo)
{
  struct winsize size;
  ioctl(fileno(stdout), TIOCGWINSZ, (char *) &size); 
  resizeterm(size.ws_row, size.ws_col);
}


int main() {
  char stop = 0;
  SwitchDir sw_dr = {0};
  char switch_window = 1;
  int driver = 0;
  DirList left_dir;
  DirList right_dir;
  int selected_button = 0;
  DriverDir(&left_dir, ".", &sw_dr);
  sw_dr.win = 1;
  DriverDir(&right_dir, ".", &sw_dr);
  sw_dr.win = 0;
  initscr();  
  start_color(); 
  WINDOW *left_window = NULL;
  WINDOW *right_window = NULL;
  WINDOW *use_window = NULL;
  signal(SIGWINCH, sig_winch);
  cbreak();  
  keypad(stdscr, TRUE); 
  curs_set(FALSE);
  refresh();
  int max_y, max_x;
  getmaxyx(stdscr, max_y, max_x); 
  int start_x = max_x / 2;  
  int width = max_x - start_x;  
  left_window = newwin(max_y, width, 0, 0); 
  right_window = newwin(max_y, width, 0, start_x); 
  draw_buttons(left_dir.namelist_dir, left_dir.len_namelist, selected_button, left_window);
  draw_buttons(right_dir.namelist_dir, right_dir.len_namelist, selected_button, right_window);
  wrefresh(right_window);  
  wrefresh(left_window); 
  use_window = left_window;
   while (1) {
    if(switch_window == 1)
    draw_buttons(left_dir.namelist_dir, left_dir.len_namelist, selected_button, use_window);  // Отрисовать кнопки
    else
    draw_buttons(right_dir.namelist_dir, right_dir.len_namelist, selected_button, use_window);
    int key = getch();  // Получить ввод пользователя
    int len_namelist = 0;
    switch (key) {
      case KEY_UP:  // Стрелка вверх
        len_namelist = sw_dr.win == 0 ? left_dir.len_namelist : right_dir.len_namelist;
        selected_button = (selected_button - 1 + len_namelist) % len_namelist;
        break;
      case KEY_DOWN:  // Стрелка вниз
        len_namelist = sw_dr.win == 0 ? left_dir.len_namelist : right_dir.len_namelist;
        selected_button = (selected_button + 1) % len_namelist;
        break;
      case 10:  
        if(switch_window == 1){
          driver = DriverDir(&left_dir, left_dir.namelist_dir[selected_button]->d_name, &sw_dr);
        }
        else {
          driver = DriverDir(&right_dir, right_dir.namelist_dir[selected_button]->d_name, &sw_dr);
        }
        refresh();
        break;
      case 9:
        selected_button = 0;
        switch_window = -switch_window;
        if(switch_window == 1){
          use_window = left_window;
          sw_dr.win = 0;
          driver = DriverDir(&left_dir, left_dir.namelist_dir[selected_button]->d_name, &sw_dr);
        } else {
          sw_dr.win = 1;
          use_window = right_window;
          driver = DriverDir(&right_dir, right_dir.namelist_dir[selected_button]->d_name, &sw_dr);
        }
        break;
      case 'q':    
        stop = 1;
        break;        
    }
    if(stop){
      break;
    }
  }   
  closedir(sw_dr.dr[0]);
  closedir(sw_dr.dr[1]);
  delwin(right_window);
  delwin(left_window);
  endwin();  
  exit(EXIT_SUCCESS);
}

