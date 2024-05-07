#include "lib_mess.h"

void ListMemRealloc(NameList *list){
  list->size = list->size * 2 - (list->size / 2);
  list->name = realloc(list->name, sizeof(char*) * list->size);
}