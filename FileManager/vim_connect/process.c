#include <sys/wait.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
// #include <termios.h>
void saveNcursesSettings(WINDOW **original_window) {
    *original_window = dupwin(stdscr);  // Дублировать текущее окно
}

void restoreNcursesSettings(WINDOW **original_window) {
    wrefresh(*original_window);  // Обновить дублированное окно
    delwin(*original_window);    // Удалить дублированное окно
}

void VimCall(char *name) {
    WINDOW *original_window = NULL;
    saveNcursesSettings(&original_window);

    int status = 0;
    pid_t pid_vim = fork();
    if (!pid_vim) {
        execl("/usr/bin/vim", "vim", name, NULL);
    }
    pid_vim = wait(&status);

    restoreNcursesSettings(&original_window);
}
