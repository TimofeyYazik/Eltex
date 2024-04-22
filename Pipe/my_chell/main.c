#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "addition_func/add_func.h"
#include "custom_type.h"

int main() {
  int num_tokens_pipe_write = 0;
  int num_tokens_pipe_read = 0;
  int pipe_message[2];
  pipe(pipe_message);
  char **tokens_pipe_write;
  char **tokens_pipe_read;
  char conveyor_commands = 0;
  int wait_return = 0;
  pid_t child_process = 0;
  int num_tokens = 0;
  input_user data = {0};
  data.size = 100;
  data.arr = malloc(data.size);
  while (1) {
    safe_input(&data);
    if (!strcmp(data.arr, "exit")) break;
    char **tokens = tokenize_string(data.arr, "|", &num_tokens);
    // printf("%d\n", num_tokens);
    if (num_tokens == 2) {
      tokens_pipe_write =
          tokenize_string(tokens[0], " ", &num_tokens_pipe_write);
      tokens_pipe_read = tokenize_string(tokens[1], " ", &num_tokens_pipe_read);
    }
    for (int i = 0; i < num_tokens; i++) {
      int num_tokens_pipe;
      char ***buff;
      child_process = fork();
      if (!child_process) {
        if (num_tokens > 1) {
          if (i == 0) {
            close(pipe_message[0]);
            dup2(pipe_message[1], 1);
            buff = &tokens_pipe_write;
            num_tokens_pipe = num_tokens_pipe_write;
          } else {
            close(pipe_message[1]);
            dup2(pipe_message[0], 0);
            buff = &tokens_pipe_read;
            num_tokens_pipe = num_tokens_pipe_read;
          }
        } else {
          *buff = tokenize_string(data.arr, " ", &num_tokens_pipe);
        }
        char *args[num_tokens_pipe + 1];
        for (int j = 0; j < num_tokens_pipe; j++) {
          args[j] = (*buff)[j];
        }
        args[num_tokens_pipe] = NULL;
        execvp((*buff)[0], args);
      } else {
        waitpid(child_process, &wait_return, 0);
        for(int j = 0; j < num_tokens_pipe; j++) {
          free((*buff)[j]);
        }
        free(*buff);
        free(tokens_pipe_write);
        free(tokens_pipe_read);
      }
    }
    for (int i = 0; i < num_tokens; i++) {
      free(tokens[i]);
    }
    free(tokens);
  }
  free(data.arr);
  exit(EXIT_SUCCESS);
}
