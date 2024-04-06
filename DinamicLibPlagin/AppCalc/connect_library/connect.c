#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

#include "../castom_type.h"
#include "connect.h"

void ConnectLib(int (***func_calc_int)(int, int), handler_t *handler, int *count, const service_info service){
  int func_to_be = 0;
  int j = 0;
  for(int i = 0; i < service.lib_count; i++){
    handler->len++;
    handler->handler_arr = realloc(handler->handler_arr, handler->len * sizeof(void *));
    handler->handler_arr[i] = dlopen(service.lib[i], RTLD_LAZY);
    if(handler->handler_arr[i] == NULL){
      fprintf(stderr, "path not found");
      exit(EXIT_FAILURE);
    }
    int *count_func = dlsym(handler->handler_arr[i], "g_number_functions");
    if(count_func == NULL){
      fprintf(stderr, "var not found");
      exit(EXIT_FAILURE);
    }
    char *func_name_ = dlsym(handler->handler_arr[i], "g_name_func");
    char **func_name = *func_name_;
    if(func_name_ == NULL){
      fprintf(stderr, "var not found");
      exit(EXIT_FAILURE);
    }
    printf("%s\n", func_name[0]);
    for(; j < service.func_count; j++){
      if(!strcmp(service.func[j], "new lib")) {
        j++;
        break;
      }
      for(int l = 0; l < *count_func; l++){
        if(!strcmp(service.func[j], (*func_name)[l])){
          func_to_be = 1;
        }
      }
      if(func_to_be){
        (*count)++;
        (*func_calc_int) = realloc(*func_calc_int, (*count * sizeof(int (*)(int, int))));
        (*func_calc_int)[*count - 1] = dlsym(handler->handler_arr[i], service.func[j]);
        if((*func_calc_int)[*count - 1] == NULL){
          fprintf(stderr, "not function\n");
          exit(EXIT_FAILURE);
        }
      }
      func_to_be = 0;
    }
  }
}