#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

#include "../castom_type.h"
#include "connect.h"

int ConnectLib(int (***func_calc_int)(int, int), Handler *handler, int *count, const ServiceInfo service){
  int error = 0;
  int j = 0;
  for(int i = 0; i < service.lib_count; i++){
    handler->len++;
    handler->handler_arr = realloc(handler->handler_arr, handler->len * sizeof(void *));
    if (handler->handler_arr == NULL) {
      fprintf(stderr, "not memmory allocaten");
      error = 1;
      goto Err;
    }
    handler->handler_arr[i] = dlopen(service.lib[i], RTLD_LAZY);
    if(handler->handler_arr[i] == NULL){
      fprintf(stderr, "path not found");
      error = 1;
      goto Err;
    }
    // int *count_func = dlsym(handler->handler_arr[i], "g_number_functions");
    // if(count_func == NULL){
    //   fprintf(stderr, "var not found");
    //   exit(EXIT_FAILURE);
    // }
    // char ***func_name = dlsym(handler->handler_arr[i], "g_name_func");
    // if(func_name == NULL){
    //   fprintf(stderr, "var not found");
    //   exit(EXIT_FAILURE);
    // } 
    // printf("%s\n", (*func_name)[0]); //в этой строке происходит seg fault 
    for(; j < service.func_count; j++){
      if(!strcmp(service.func[j], "new lib")) {
        j++;
        break;
      }
        (*count)++;
        (*func_calc_int) = realloc(*func_calc_int, (*count * sizeof(int (*)(int, int))));
        if((*func_calc_int) == NULL){
          fprintf(stderr, "not memmory allocaten");
          error = 1;
          goto Err;
        }
        (*func_calc_int)[*count - 1] = dlsym(handler->handler_arr[i], service.func[j]);
        if((*func_calc_int)[*count - 1] == NULL){
          fprintf(stderr, "not function\n");
          error = 1;
          goto Err;
        }
    }
  }
  Err:
  return error;
}
