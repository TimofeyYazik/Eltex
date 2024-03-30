#include "iofunc.h"

#include <stdio.h>
#include <string.h>

#include "additional_functions.h"
#include "logic_func.h"
#include "castom_type.h"

int MenuBar() {
  int menu = 0;
  printf(
      "1. new Abonent\n"
      "2. delete Abonent\n"
      "3. search Abonent\n"
      "4. print Abonent\n"
      "5. exit\n");
  CheckErrorScanf(scanf("%d", &menu), 1);
  ClearStdin();
  return menu;
}

void PrintAbonent(AbonentMas mas){
  for(int i = 0; i < mas.size; i++){
    printf("Name: %10s\n", mas.abonent_mas[i].name);
    printf("Second name: %3s\n", mas.abonent_mas[i].second_name);
    printf("Tell: %10s\n", mas.abonent_mas[i].tell);
  }
}

