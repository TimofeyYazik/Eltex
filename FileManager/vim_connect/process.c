#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <unistd.h>

void VimCall(char *name) {
  int status = 0;
  pid_t pid_vim = fork();
  if (pid_vim == -1) {
    perror("Error in fork");
    return;
  }
  if (!pid_vim) {
    execl("/usr/bin/vim", "vim", name, NULL);
    perror("Error in execl");
    exit(EXIT_FAILURE);
  }
  if (wait(&status) == -1) {
    perror("Error in wait");
    return;
  }
}
