#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

#include "../castom_type.h"
#include "connect.h"

void ConnectLib(int (***func_calc_int)(int, int), int *count, const service_info service){
  // int func_to_be = 0;
  for(int i = 0; i < service.lib_count; i++){
    printf("%s\n", service.lib[i]);
    void *handler_lib = dlopen(service.lib[i], RTLD_LAZY);
    if(handler_lib == NULL){
      fprintf(stderr, "path not found");
      exit(EXIT_FAILURE);
    }
    int *count_func = dlsym(handler_lib, "g_number_functions");
    // char ***func_name = dlsym(handler_lib, "g_name_func");
    printf("%d\n", *count_func);
    // printf("%s\n",(*func_name)[0]);
    for(int j = 0; j < service.func_count; j++){
      if(!strcmp(service.func[j], "new lib")) {
        break;
      }
      // for(int l = 0; l < *count_func; l++){
      //   if(!strcmp(service.func[j], (*func_name)[l])){
      //     func_to_be = 1;
      //   }
      // }
      // if(func_to_be){
        (*count)++;
        (*func_calc_int) = realloc(*func_calc_int, (*count * sizeof(int (*)(int, int))));
        (*func_calc_int)[*count - 1] = dlsym(handler_lib, service.func[j]);
        if((*func_calc_int)[*count - 1] == NULL){
          fprintf(stderr, "not function\n");
          exit(EXIT_FAILURE);
        }
      // }
      // func_to_be = 0;
    }
    // dlclose(handler_lib);
  }
}