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
  char **tokens_pipe_write;
  char **tokens_pipe_read;
  int wait_return = 0;
  pid_t child_pid = 0;
  int num_tokens = 0;
  input_user data = {0};
  data.size = 100;
  data.arr = malloc(data.size);
  if (data.arr == NULL) {
    perror("Memory allocation failed");
    exit(EXIT_FAILURE);
  }
  while (1) {
    SafeInput(&data);
    if (!strcmp(data.arr, "exit")) break;
    char **tokens = TokenizeString(data.arr, "|", &num_tokens);
    if (num_tokens == 2) {
    if (pipe(pipe_message) == -1) {
      perror("Pipe creation failed");
      exit(EXIT_FAILURE);
    }
      tokens_pipe_write =
          TokenizeString(tokens[0], " ", &num_tokens_pipe_write);
      tokens_pipe_read = TokenizeString(tokens[1], " ", &num_tokens_pipe_read);
    }
    for (int i = 0; i < num_tokens; i++) {
      int num_tokens_pipe;
      char ***buff;
      child_pid = fork();
      if (child_pid == -1) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
      }
      if (child_pid == 0) {
        if (num_tokens > 1) {
          if (i == 0) {
            close(pipe_message[0]);
            dup2(pipe_message[1], 1);
            buff = &tokens_pipe_write;
            num_tokens_pipe = num_tokens_pipe_write;
          } else if (i == 1) {
            close(pipe_message[1]);
            dup2(pipe_message[0], 0);
            buff = &tokens_pipe_read;
            num_tokens_pipe = num_tokens_pipe_read;
          }
        } else {
          *buff = TokenizeString(data.arr, " ", &num_tokens_pipe);
        }
        char *args[num_tokens_pipe + 1];
        for (int j = 0; j < num_tokens_pipe; j++) {
          args[j] = (*buff)[j];
        }
        args[num_tokens_pipe] = NULL;
        execvp((*buff)[0], args);
        perror("Execution failed");
        exit(EXIT_FAILURE);
      } else {
        waitpid(child_pid, &wait_return, 0);
        if(i == 0) {
          close(pipe_message[1]);
        } else if(i == 1) {
          close(pipe_message[0]);
        }
      }
    }
    for (int i = 0; i < num_tokens; i++) {
      free(tokens[i]);
    }
    free(tokens);
    if (num_tokens == 2) {
      for (int i = 0; i < num_tokens_pipe_write; i++) {
        free(tokens_pipe_write[i]);
      }
      free(tokens_pipe_write);
      for (int i = 0; i < num_tokens_pipe_read; i++) {
        free(tokens_pipe_read[i]);
      }
      free(tokens_pipe_read);
    }
  }
  free(data.arr);
  exit(EXIT_SUCCESS);
}
