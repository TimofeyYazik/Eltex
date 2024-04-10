// #include <curses.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <dirent.h>
#include <ncurses.h>
#include <string.h>

#include "driver_dir/driver.h"

#define TRUE 1
#define FALSE 0

void draw_buttons(struct dirent **namelist_dir, int count, int selected) {
  clear();  // Очистить экран
  for (int i = 0; i < count; ++i) {
    if (i == selected) {
      attron(A_REVERSE);  // Включить обратный вид (выделение выбранной кнопки)
    }
    mvprintw(i + 1, 1, "%s", namelist_dir[i]->d_name);  // Вывести кнопку на экран
    if (i == selected) {
      attroff(A_REVERSE);  // Выключить обратный вид
    }
  }
  refresh();  // Обновить экран
}

int main() {
  // WINDOW *addition_window = NULL;
  int len_namelist = 0;
  struct dirent **namelist_dir;
  int selected_button = 0;
  DriverDir(&len_namelist, &namelist_dir, ".");
  initscr();  // Инициализировать ncurses
  // addition_window = newwin(80, 100, 2, 2);
  cbreak();  // Включить режим CBREAK (ввод без буферизации, но с сигналами прерывания)
  keypad(stdscr, TRUE);  // Включить использование специальных клавиш
  curs_set(FALSE);
  while (1) {
    draw_buttons(namelist_dir, len_namelist, selected_button);  // Отрисовать кнопки
    int key = getch();  // Получить ввод пользователя
    switch (key) {
      case KEY_UP:  // Стрелка вверх
        selected_button = (selected_button - 1 + len_namelist) % len_namelist;
        break;
      case KEY_DOWN:  // Стрелка вниз
        selected_button = (selected_button + 1) % len_namelist;
        break;
      case 10:  // Enter
        DriverDir(&len_namelist, &namelist_dir, namelist_dir[selected_button]->d_name);
        refresh();
        break;
      case 'q':    // Нажата клавиша 'q'
        endwin();  // Завершить работу с ncurses
        return 0;
    }
  }

  endwin();  // Завершить работу с ncurses
  return 0;
}
