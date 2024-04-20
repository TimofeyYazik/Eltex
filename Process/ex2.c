#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  int status = 0;
  pid_t child_process_branch1 = fork();
  if (child_process_branch1 == 0) {
    printf("Child pid - %d\n", getpid());
    pid_t child_process_branch1_1 = fork();
    if (child_process_branch1_1 == 0) {
      printf("Child pid - %d\n", getpid());
    } else {
      waitpid(child_process_branch1_1, &status, 0);
      printf("Parent pid - %d\n", getpid());
    }
  } else {
    waitpid(child_process_branch1, &status, 0);
    printf("Parent pid - %d\n", getpid());
  }
  pid_t child_process_branch2 = fork();
  if (child_process_branch2 == 0) {
    printf("Child pid - %d\n", getpid());
    pid_t child_process_branch2_1 = fork();
    if (child_process_branch2_1 == 0) {
      printf("Child pid - %d\n", getpid());
    } else {
      pid_t child_process_branch2_2 = fork();
      if (child_process_branch2_2 == 0) {
        printf("Child pid - %d\n", getpid());
      } else {
        printf("Parent pid - %d\n", getpid());
        waitpid(child_process_branch2_1, &status, 0);
        waitpid(child_process_branch2_2, &status, 0);
      }
    }
  } else {
    waitpid(child_process_branch2, &status, 0);
    printf("Parent pid - %d\n", getpid());
  }
  exit(EXIT_SUCCESS);
}