#include <signal.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <ncurses.h>
#include <string.h>
#include <sys/wait.h>

#include "driver_dir/driver.h"

#define TRUE 1
#define FALSE 0

void saveTerminalSettings(struct termios *original_termios) {
    tcgetattr(STDIN_FILENO, original_termios);
}

void restoreTerminalSettings(const struct termios *original_termios) {
    tcsetattr(STDIN_FILENO, TCSANOW, original_termios);
}

void VimCall(char *name) {
    struct termios original_termios;
    saveTerminalSettings(&original_termios);
    int status = 0;
    pid_t pid_vim = fork();
    if (!pid_vim) {
        execl("/usr/bin/vim", "vim", name, NULL);
        // Если execl() завершилась с ошибкой, выводим сообщение и завершаем дочерний процесс
        perror("execl failed");
        exit(EXIT_FAILURE);
    } else if (pid_vim < 0) {
        // Если fork() завершилась с ошибкой, выводим сообщение и завершаем программу
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    // Ждем завершения процесса vim и получаем его статус
    printw("Waiting for vim process to exit...\n");
    refresh();
    waitpid(pid_vim, &status, 0);
    printw("Vim process exited\n");
    if (WIFEXITED(status)) {
        // Если процесс завершился успешно, выводим его статус
        printw("vim exited with status %d\n", WEXITSTATUS(status));
    } else {
        // Если процесс завершился с ошибкой, выводим сообщение об ошибке
        printw("vim exited with error\n");
    }
    fflush(stdout); // Сбрасываем буфер вывода на экран

    restoreTerminalSettings(&original_termios);
}




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
  int driver = 0;
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
        driver = DriverDir(&len_namelist, &namelist_dir, namelist_dir[selected_button]->d_name);
        if(driver == ENOTDIR){
          VimCall(namelist_dir[selected_button]->d_name);
        }
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


