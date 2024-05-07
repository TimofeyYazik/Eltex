#include "lib_mess.h"

void ShiftList(NameList *list, int index){
  for(int i = index; i < list->len - 1; i++){
    strcpy(list->name[i], list->name[i + 1]);
  }
  list->len--;
  free(list->name[list->len]);
  list->name[list->len] = NULL;
}