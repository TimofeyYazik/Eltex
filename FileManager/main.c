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

int main() {
  WINDOW *addition_window = NULL;
  int len_namelist = 0;
  int driver = 0;
  struct dirent **namelist_dir;
  int selected_button = 0;
  initscr();  // Инициализировать ncurses
  cbreak();  // Включить режим CBREAK (ввод без буферизации, но с сигналами прерывания)
  keypad(stdscr, TRUE);  // Включить использование специальных клавиш
  curs_set(FALSE);
  addition_window = newwin(5, 18, 2, 4);
  box(addition_window, '|', '-');
  wattron(addition_window, COLOR_YELLOW);
  mvwprintw(addition_window, 1, 1, "Hello");  // Вывод текста в окно
  wrefresh(addition_window);  // Обновление экрана
  // Добавляем задержку и сообщение
  printw("Нажмите любую клавишу для завершения...");
  getch();  // Ожидаем нажатие клавиши
  delwin(addition_window);  // Удаление окна
  endwin();  // Завершить работу с ncurses
  return 0;
}




//  while (1) {
//     draw_buttons(namelist_dir, len_namelist, selected_button);  // Отрисовать кнопки
//     int key = getch();  // Получить ввод пользователя
//     switch (key) {
//       case KEY_UP:  // Стрелка вверх
//         selected_button = (selected_button - 1 + len_namelist) % len_namelist;
//         break;
//       case KEY_DOWN:  // Стрелка вниз
//         selected_button = (selected_button + 1) % len_namelist;
//         break;
//       case 10:  // Enter
//         driver = DriverDir(&len_namelist, &namelist_dir, namelist_dir[selected_button]->d_name);
//         if(driver == ENOTDIR){
//           //VimCall(namelist_dir[selected_button]->d_name);
//         }
//         refresh();
//         break;
//       case 'q':    // Нажата клавиша 'q'
//         endwin();  // Завершить работу с ncurses
//         return 0;
//     }
//   }