#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// #include <malloc.h>
#include <errno.h>

#include "logic_func.h"

// #include "iofunc.h"
#include "castom_type.h"
#include "additional_functions.h"


void NewAbonent(AbonentMas *mas) {
  if (mas->size == 0) {
    mas->abonent_mas = calloc(1, sizeof(Abonent));
    CheckErrorMemory(*mas);
    mas->size = 1;
  } else {
    mas->size++;
    mas->abonent_mas = realloc(mas->abonent_mas, sizeof(Abonent) * mas->size);
    CheckErrorMemory(*mas);
  }
  CheckErrorScanf(scanf("%9s", mas->abonent_mas[mas->size - 1].name), 1);
  ClearStdin();
  CheckErrorScanf(scanf("%9s", mas->abonent_mas[mas->size - 1].second_name), 1);
  ClearStdin();
  CheckErrorScanf(scanf("%9s", mas->abonent_mas[mas->size - 1].tell), 1);
  ClearStdin();
}

void SearchAbonent(AbonentMas *mas){
  char search_name[10] = {0};
  int search_ready = -1;
  CheckErrorScanf(scanf("%9s", search_name), 1);
  ClearStdin();
  for(int i = 0; i < mas->size; i++){
    if(!strcmp(search_name, mas->abonent_mas[i].name)){
      search_ready = i;
      break;
    }
  }
  if(search_ready == -1){
    printf("abonent not found");
  } else {
    printf("Name: %10s\n", mas->abonent_mas[search_ready].name);
    printf("Second name: %3s\n", mas->abonent_mas[search_ready].second_name);
    printf("Tell: %10s\n", mas->abonent_mas[search_ready].tell);
  }
}

void DelAbonent(AbonentMas *mas){
  char del_name[10] = {0};
  int search_ready = 0;
  CheckErrorScanf(scanf("%9s", del_name), 1);
  ClearStdin();
  for(int i = 0; i < mas->size; i++){
    if(!strcmp(del_name, mas->abonent_mas[i].name) && !search_ready){
      search_ready = 1;
      continue;
    }
    if(search_ready){
        CopyAbonent(&mas->abonent_mas[i - 1], &mas->abonent_mas[i]);
    }
  }
  // PrintAbonent(*mas);
  if(search_ready){
    mas->size--;
    mas->abonent_mas = realloc(mas->abonent_mas, mas->size);
    CheckErrorMemory(*mas);
    // PrintAbonent(*mas);
  } else {
    printf("abonent not found");
  }
}

