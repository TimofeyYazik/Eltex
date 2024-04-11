#include <sys/wait.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>
// #include <termios.h>

void saveNcursesSettings(SCREEN *original_screen) {
    def_prog_mode();  // Сохранить настройки экрана ncurses
    refresh();  // Обновить экран, чтобы изменения вступили в силу
    original_screen = newterm(NULL, stdout, stdin);  // Создать новый экран для восстановления
}

void restoreNcursesSettings(SCREEN *original_screen) {
    endwin();  // Завершить работу с текущим экраном
    set_term(original_screen);  // Восстановить сохраненные настройки экрана
    reset_prog_mode();  // Восстановить сохраненное состояние экрана
    refresh();  // Обновить экран
}

void VimCall(char *name) {
    SCREEN *original_screen = NULL;
    saveNcursesSettings(original_screen);

    int status = 0;
    pid_t pid_vim = fork();
    if (!pid_vim) {
        execl("/usr/bin/vim", "vim", name, NULL);
    }
    pid_vim = wait(&status);

    restoreNcursesSettings(original_screen);
}
