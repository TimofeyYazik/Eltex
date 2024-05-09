#include "lib_mess.h"

void AddNameList(NameList *src, char *name){
  if(src->name[src->len] == NULL) src->name[src->len] = malloc(char * MAX_NAME_LEN);
  strcpy(src->name[src->len], name);
  src->len++;
  if(src->len == src->size) {
    ListMemRealloc(src);
    for(int i = src->len; i < src->size; i++){
      src->name[i] = NULL;
    }
  }
}