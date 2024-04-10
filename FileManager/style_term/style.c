#include <ncurses.h>
#include <dirent.h>

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