#include <ncurses.h>
#include <dirent.h>
#include <sys/stat.h>

void draw_buttons(struct dirent **namelist_dir, int count, int selected, WINDOW *window_name) {
  init_pair(1, COLOR_MAGENTA, COLOR_BLACK); 
  init_pair(2, COLOR_CYAN, COLOR_BLACK);
  struct stat statbuf;
  werase(window_name);   // Очистить экран
  box(window_name, '|', '-');
  for (int i = 0; i < count; ++i) {
    stat(namelist_dir[i]->d_name, &statbuf);
    if (i == selected) {
      if(S_ISREG(statbuf.st_mode))
        wattron(window_name,  COLOR_PAIR(1) | A_REVERSE);
      else 
        wattron(window_name, COLOR_PAIR(2) | A_REVERSE);
    }
    mvwprintw(window_name, i + 1, 1, "%s", namelist_dir[i]->d_name);  
    if (i == selected) {
      if(S_ISREG(statbuf.st_mode))
        wattroff(window_name,  COLOR_PAIR(1) | A_REVERSE);
      else 
        wattroff(window_name, COLOR_PAIR(2) | A_REVERSE); 
    }
  }
  wrefresh(window_name);  
}