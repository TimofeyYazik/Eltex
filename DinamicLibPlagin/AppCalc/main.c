#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

#include "castom_type.h"
#include "parse/parse.h"



int main(int argc, char **argv){
  int (*func_calc_int)(int, int);
  float (*func_calc_float)(float, float);
  int *count_func = 0;
  char ***func_name = NULL;
  service_info servis = {0};
  parse_flags(&servis, argc, argv);
  // printf("%s\n", servis.lib);
  // for(int i = 0; i < servis.func_count; i++){
  //   printf("%s\n", servis.func[i]);
  // }
  // void *handler_lib = dlopen(servis.lib, RTLD_LAZY);
  // if(handler_lib == NULL){
  //   fprintf(stderr, "path not found");
  //   exit(EXIT_FAILURE);
  // }
  // count_func = dlsym(handler_lib, "g_number_functions");
  // func_name = dlsym(handler_lib, "g_name_func");


  // for(int i = 0; i < servis.func_count; i++){
  //   free(servis.func[i]);
  // }
  // free(servis.func);
  return 0;
}