#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

typedef struct 
{
  int size;
  int len_str;
  char *arr;
}input_user;

void safe_input(input_user *data){
  char ch = 0;
  while ((ch = getchar()) != '\n'){
    if(data->len_str == data->size){
      data->size *= 2;
    }
    data->arr[data->len_str] = ch;
    data->len_str++;
  }
}

char **tokenize_string(const char *str, const char *delimiters, int *num_tokens) {
    char *str_copy = strdup(str); // Создаем копию строки, чтобы не изменять исходную строку
    if (str_copy == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    // Вычисляем количество токенов
    *num_tokens = 0;
    char *token = strtok(str_copy, delimiters);
    while (token != NULL) {
        (*num_tokens)++;
        token = strtok(NULL, delimiters);
    }

    // Выделяем память под массив указателей на токены
    char **tokens = malloc((*num_tokens) * sizeof(char *));
    if (tokens == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    // Снова разбиваем строку и сохраняем каждый токен в массиве tokens
    int i = 0;
    token = strtok(strdup(str), delimiters);
    while (token != NULL) {
        tokens[i++] = strdup(token);
        token = strtok(NULL, delimiters);
    }

    free(str_copy); // Освобождаем память, выделенную для временной копии строки
    return tokens;
}

int main(){
  int wait_return;
  pid_t child_process;
  int num_tokens;
  input_user data = {0};
  const char folder[6] = "/bin/";
  char utils_path[50] = {0};
  data.size = 100;
  data.arr = malloc(data.size);
  while (1){
    safe_input(&data);
    if(!strcmp(data.arr, "exit")) break;
    char **tokens = tokenize_string(data.arr, " ", &num_tokens);
    child_process = fork();
    if(!child_process){
      sprintf(utils_path, "%s%s", folder, tokens[0]);
      switch (num_tokens)
      {
      case 1:
         execl(utils_path, tokens[0], NULL);
        break;
      case 2:
         execl(utils_path, tokens[0], tokens[1], NULL);
        break;
      case 3:
         execl(utils_path, tokens[0], tokens[1], tokens[2], NULL);
        break;
      case 4:
         execl(utils_path, tokens[0], tokens[1], tokens[2], tokens[3], NULL);
        break;
      case 5:
         execl(utils_path, tokens[0], tokens[1], tokens[2], tokens[3], tokens[4], NULL);
        break;
      case 6:
         execl(utils_path, tokens[0], tokens[1], tokens[2], tokens[3], tokens[4], tokens[5], NULL);
        break;
      default:
        break;
      } 
    } else {
      wait(&wait_return);
    }
    for(int i = 0; i < num_tokens; i++){
      free(tokens[i]);
    }
    free(tokens);
  }
  free(data.arr);
  exit(EXIT_SUCCESS);
}