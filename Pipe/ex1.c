#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(){
  pid_t child_procces;
  int pipe_[2];
  pipe(pipe_);
  child_procces = fork();
  if(child_procces == 0){
    close(pipe_[1]);
    char buf[256];
    read(pipe_[0], buf, 256);
    printf("%s\n", buf);
    close(pipe_[0]);
    exit(EXIT_SUCCESS);
  } else {
    close(pipe_[0]);
    char buf[256] = "Hello World!";
    write(pipe_[1], buf, 256);
    wait(NULL);
    close(pipe_[1]);
  }
  exit(EXIT_SUCCESS);
}