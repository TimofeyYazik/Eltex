#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  int status = 0;
  pid_t child_process = fork();
  if (child_process == 0) {
    printf("Child pid - %d\n", getpid());
    execl("/bin/ls", "ls", "-l", NULL);
  } else if (child_process == -1) {
    perror("process not make\n");
    exit(EXIT_FAILURE);
  } else {
    printf("Parent pid - %d\n", getpid());
    wait(&status);
    printf("Status - %d\n", WEXITSTATUS(status));
  }
  exit(EXIT_SUCCESS);
}