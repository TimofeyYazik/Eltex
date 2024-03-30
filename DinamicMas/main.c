#include <stdio.h>
#include <stdlib.h>
//#include <malloc.h>

#include "castom_type.h"
#include "iofunc.h"
#include "logic_func.h"

int main() {
  AbonentMas mas = {0};
  volatile int argument = 0;
  while (argument != 5) {
    argument = MenuBar();
    switch (argument) {
      case 1:
        NewAbonent(&mas);
        break;
      case 2:
        DelAbonent(&mas);
        break;
      case 3:
        SearchAbonent(&mas);
        break;
      case 4:
        PrintAbonent(mas);
        break;
      case 5:
        break;
      default:
        printf("operation not found\n");
        break;
    }
  }
  free(mas.abonent_mas);
  return 0;
}
