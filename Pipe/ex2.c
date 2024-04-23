#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(){
  mkfifo("my_pipe", 0666);
  pid_t child_procces = fork();
  if(child_procces == 0){
    char buf[256];
    int pipe_named = open("my_pipe", O_RDONLY);
    read(pipe_named, buf, 256);
    printf("%s\n", buf);
    close(pipe_named);
    exit(EXIT_SUCCESS);
  } else {
    char buf[256] = "Hello World!";
    int pipe_named = open("my_pipe", O_WRONLY);
    write(pipe_named, buf, 256);
    close(pipe_named);
    waitpid(child_procces, NULL, WEXITED);
    unlink("my_pipe");
  }
  exit(EXIT_SUCCESS);
}