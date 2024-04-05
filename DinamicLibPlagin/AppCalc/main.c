#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

#include "castom_type.h"
#include "parse/parse.h"
#include "menu_work/menu.h"
#include "connect_library/connect.h"


int main(int argc, char **argv){
  int count_func_arr = 0;
  int (**func_calc_int)(int, int) = NULL;
  int var_func_one = 0, var_func_two = 0;
  volatile int argument = 1;
  service_info service = {0};
  ParseFlags(&service, argc, argv);
  ConnectLib(func_calc_int, &count_func_arr, service);
  while (argument) {
    argument = Menu(service);
    if (argument > 0 && argument < count_func_arr) {
      scanf("%d %d", &var_func_one, &var_func_two);
      func_calc_int[argument - 1](var_func_one, var_func_two);
    } else {
      if(!argument) continue;
      printf("not this argument");
    }
  }
  
  return 0;
}

