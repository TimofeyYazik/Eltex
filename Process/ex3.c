#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

typedef struct {
  int size;
  int len_str;
  char *arr;
} input_user;

void safe_input(input_user *data) {
  memset(data->arr, 0, data->len_str);
  char ch = 0;
  while ((ch = getchar()) != '\n') {
    if (data->len_str == data->size) {
      data->size *= 2;
      data->arr = realloc(
          data->arr,
          data->size);  // Используем realloc для изменения размера буфера
    }
    data->arr[data->len_str] = ch;
    data->len_str++;
  }
}

char **tokenize_string(const char *str, const char *delimiters,
                       int *num_tokens) {
  char *str_copy = strdup(str);
  if (str_copy == NULL) {
    perror("Memory allocation error");
    exit(EXIT_FAILURE);
  }

  *num_tokens = 0;
  char *token = strtok(str_copy, delimiters);
  while (token != NULL) {
    (*num_tokens)++;
    token = strtok(NULL, delimiters);
  }

  char **tokens = malloc((*num_tokens) * sizeof(char *));
  if (tokens == NULL) {
    perror("Memory allocation error");
    exit(EXIT_FAILURE);
  }

  int i = 0;
  token = strtok(strdup(str), delimiters);
  while (token != NULL) {
    tokens[i++] = strdup(token);
    token = strtok(NULL, delimiters);
  }

  free(str_copy);
  return tokens;
}

int main() {
  int wait_return;
  pid_t child_process;
  int num_tokens;
  input_user data = {0};
  const char folder[6] = "/bin/";
  char utils_path[50] = {0};
  data.size = 100;
  data.arr = malloc(data.size);
  while (1) {
    safe_input(&data);
    if (!strcmp(data.arr, "exit")) break;
    char **tokens = tokenize_string(data.arr, " ", &num_tokens);
    child_process = fork();
    if (!child_process) {
      sprintf(utils_path, "%s%s", folder, tokens[0]);
      char *args[num_tokens + 1];
      for (int i = 0; i < num_tokens; i++) {
        args[i] = tokens[i];
      }
      args[num_tokens] = NULL;
      execv(utils_path, args);
      perror("Execution failed");
      exit(EXIT_FAILURE);
    } else {
      wait(&wait_return);
    }
    for (int i = 0; i < num_tokens; i++) {
      free(tokens[i]);
    }
    free(tokens);
  }
  free(data.arr);
  exit(EXIT_SUCCESS);
}
