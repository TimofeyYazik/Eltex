#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "add_func.h"

void safe_input(input_user *data) {
  memset(data->arr, 0, data->len_str);
  data->len_str = 0;
  char ch = 0;
  while ((ch = getchar()) != '\n') {
    if (data->len_str == data->size) {
      data->size *= 2;
      data->arr = realloc(data->arr, data->size);  
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