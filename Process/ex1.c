#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
  int status = 0;
  pid_t child_process = fork();
  if(child_process == 0){
    printf("Child pid - %d\n", getpid());
    execl("/bin/ls", "-la", NULL);
  } else if(child_process == -1){
    perror("process not make\n");
    exit(EXIT_FAILURE);
  } else {
    printf("Parent pid - %d\n", getpid());
    wait(&status);
    printf("Status - %d\n", WEXITSTATUS(status));
  }
  exit(EXIT_SUCCESS);
}