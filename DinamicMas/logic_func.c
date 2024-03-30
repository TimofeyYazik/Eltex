#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "logic_func.h"

#include "castom_type.h"
#include "additional_functions.h"


void NewAbonent(AbonentMas mas) {
  if (mas.size == 0) {
    mas.abonent_mas = calloc(1, sizeof(Abonent));
    CheckErrorMemory(mas);
    mas.size = 1;
  } else {
    mas.size++;
    mas.abonent_mas = realloc(mas.abonent_mas, sizeof(Abonent) * mas.size);
    CheckErrorMemory(mas);
  }
  scanf("%9s", mas.abonent_mas[mas.size - 1].name);
  ClearStdin();
  scanf("%9s", mas.abonent_mas[mas.size - 1].second_name);
  ClearStdin();
  scanf("%9s", mas.abonent_mas[mas.size - 1].tell);
  ClearStdin();
}

void SearchAbonent(AbonentMas mas){
  char search_name[10] = {0};
  int search_ready = -1;
  scanf("%9s", search_name);
  ClearStdin();
  for(int i = 0; i < mas.size; i++){
    if(!strcmp(search_name, mas.abonent_mas[i].name)){
      search_ready = i;
      break;
    }
  }
  if(search_ready == -1){
    printf("abonent not found");
  } else {
    printf("%s\n", mas.abonent_mas[search_ready].name);
    printf("%s\n", mas.abonent_mas[search_ready].second_name);
    printf("%s\n", mas.abonent_mas[search_ready].tell);
  }
}

void DelAbonent(AbonentMas mas){
  char del_name[10] = {0};
  int search_ready = 0;
  Abonent buff = {0};
  scanf("%9s", del_name);
  ClearStdin();
  for(int i = 0; i < mas.size; i++){
    if(!strcmp(del_name, mas.abonent_mas[i].name)){
      search_ready = 1;
      continue;
    }
    if(search_ready){
        memcpy(mas.abonent_mas[i - 1].name, mas.abonent_mas[i].name, 10);
        memcpy(mas.abonent_mas[i - 1].second_name, mas.abonent_mas[i].second_name, 10);
        memcpy(mas.abonent_mas[i - 1].tell, mas.abonent_mas[i].tell, 10);
    }
  }
  if(search_ready){
    mas.size--;
    mas.abonent_mas = realloc(mas.abonent_mas, mas.size);
  } else {
    printf("abonent not found");
  }
}

