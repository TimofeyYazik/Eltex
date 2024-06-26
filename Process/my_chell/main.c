#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "custom_type.h"
#include "addition_func/add_func.h"

int main() {
  int wait_return = 0;
  pid_t child_process = 0;
  int num_tokens = 0;
  input_user data = {0};
  data.size = 100;
  data.arr = malloc(data.size);
  while (1) {
    SafeInput(&data);
    if (!strcmp(data.arr, "exit")) break;
    char **tokens = TokenizeString(data.arr, " ", &num_tokens);
    child_process = fork();
    if (!child_process) {
      char *args[num_tokens + 1];
      for (int i = 0; i < num_tokens; i++) {
        args[i] = tokens[i];
      }
      args[num_tokens] = NULL;
      execvp(tokens[0], args);
      exit(EXIT_FAILURE);
    } else {
      waitpid(child_process, &wait_return, 0);
      wait_return = 0;
    }
    for (int i = 0; i < num_tokens; i++) {
      free(tokens[i]);
    }
    free(tokens);
  }
  free(data.arr);
  exit(EXIT_SUCCESS);
}
