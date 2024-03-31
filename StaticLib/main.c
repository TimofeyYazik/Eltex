#include <stdio.h>
#include "calclib.h"

int main(){
  int menu = 0;
  int res = 0, var1 = 0, var2 = 0;
  int flag = 0;
  while (menu != 5) {
    if(flag) {
      printf("result: %d", res);
    }
    printf("1) Add\n"
    "2) Sub\n"
    "3) Mult\n"
    "4) Div\n"
    "5) Exit\n");
    scanf("%d", &menu);
    switch (menu) {
    case 1:
      scanf("%d %d", &var1, &var2);
      res = Add(var1, var2);
      break;
    case 2:
      scanf("%d %d", &var1, &var2);
      res = Sub(var1, var2);
      break;
    case 3:
      scanf("%d %d", &var1, &var2);
      res = Mul(var1, var2);
      break;
    case 4:
      scanf("%d %d", &var1, &var2);
      res = Div(var1, var2);
      break;
    case 5:
      break;
    default:
      break;
    }
    flag = 1;
  }
  
}