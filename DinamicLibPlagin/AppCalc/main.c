#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

#include "castom_type.h"
#include "parse/parse.h"
#include "menu_work/menu.h"
#include "connect_library/connect.h"

void ClearService(service_info *service);
void CloseHandler(handler_t *handler);

int main(int argc, char **argv){
  int count_func_arr = 0;
  int (**func_calc_int)(int, int) = NULL;
  handler_t handler = {0};
  int var_func_one = 0, var_func_two = 0;
  int result = 0;
  volatile int argument = 1;
  service_info service = {0};
  ParseFlags(&service, argc, argv);
  ConnectLib(&func_calc_int, &handler, &count_func_arr,  service);
  while (argument) {
    argument = Menu(service);
    if (argument > 0 && argument <= count_func_arr) {
      scanf("%d %d", &var_func_one, &var_func_two);
      result = func_calc_int[argument - 1](var_func_one, var_func_two);
      printf("%d\n", result);
    } else {
      if(!argument) continue;
      printf("not this argument\n");
    }
  }

  free(func_calc_int);
  ClearService(&service);
  CloseHandler(&handler);
  return 0;
}

void CloseHandler(handler_t *handler){
  for(int i = 0; i < handler->len; i++){
    dlclose(handler->handler_arr[i]);
  }
  free(handler->handler_arr);
}

void ClearService(service_info *service){
  for(int i = 0; i < service->func_count; i++){
    free(service->func[i]);
  }
  free(service->func);
  for(int i = 0; i < service->lib_count; i++){
    free(service->lib[i]);
  }
  free(service->lib);
}
