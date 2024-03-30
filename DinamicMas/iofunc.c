#include "iofunc.h"

#include <stdio.h>
// #include <malloc.h>
#include <string.h>

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
  scanf("%d", &menu);
  ClearStdin();
  return menu;
}

void PrintAbonent(AbonentMas mas){
  for(int i = 0; i < mas.size; i++){
    printf("%s", mas.abonent_mas[i].name);
    printf("%s", mas.abonent_mas[i].second_name);
    printf("%s", mas.abonent_mas[i].tell);
  }
}

