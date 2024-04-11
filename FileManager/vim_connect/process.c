#include <sys/wait.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdio.h>
// #include <ncurses.h>
#include <termios.h>

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
    }
    pid_vim = wait(&status);

    restoreTerminalSettings(&original_termios);
}
