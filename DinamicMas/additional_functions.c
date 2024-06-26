#include <stdio.h>
#include <stdlib.h>
// #include <malloc.h> //not found from Linux and MacOS
#include <errno.h>

#include "additional_functions.h"

#include "castom_type.h"

void CheckErrorMemory(AbonentMas mas){
  if(mas.abonent_mas == NULL || errno == ENOMEM){
    perror("error");
    exit(EXIT_FAILURE);
  }
}

void CheckErrorScanf(int real_result, int expected_result){
  if(real_result != expected_result){
    fprintf(stderr, "type error\n");
    exit(EXIT_FAILURE);
  }
}

void ClearStdin() {
  char c = 0;
  while ((c = getchar()) != '\n' && c != EOF) {}
}

void CopyAbonent(Abonent *receiver, const Abonent *sender){
  for(int i = 0; i < SIZE_ELEMENTS; i++){
    receiver->name[i] = sender->name[i];
  }
  for(int i = 0; i < SIZE_ELEMENTS; i++){
    receiver->second_name[i] = sender->second_name[i];
  }
  for(int i = 0; i < SIZE_ELEMENTS; i++){
    receiver->tell[i] = sender->tell[i];
  }
}
