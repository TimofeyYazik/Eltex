#include <stdio.h>
#include <stdlib.h>

#include "libcalc.h"

void CheckErrorScanf(int real_result, int expected_result){
  if(real_result != expected_result){
    fprintf(stderr, "type error\n");
    exit(EXIT_FAILURE);
  }
}

int main(){
  int menu = 0;
  int res = 0, var1 = 0, var2 = 0;
  int flag = 0;
  while (menu != 5) {
    if(flag) {
      printf("result: %d\n", res);
    }
    printf("1) Add\n"
    "2) Sub\n"
    "3) Mult\n"
    "4) Div\n"
    "5) Exit\n");
    CheckErrorScanf(scanf("%d", &menu), 1);
    switch (menu) {
    case 1:
      CheckErrorScanf(scanf("%d %d", &var1, &var2), 2);
      res = Add(var1, var2);
      break;
    case 2:
      CheckErrorScanf(scanf("%d %d", &var1, &var2), 2);
      res = Sub(var1, var2);
      break;
    case 3:
      CheckErrorScanf(scanf("%d %d", &var1, &var2), 2);
      res = Mul(var1, var2);
      break;
    case 4:
      CheckErrorScanf(scanf("%d %d", &var1, &var2), 2);
      res = Div(var1, var2);
      break;
    case 5:
      break;
    default:
      printf("operation not found\n");
      break;
    }
    flag = 1;
  }
  return 0;
}