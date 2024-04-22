#include "add_func.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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