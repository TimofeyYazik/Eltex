#include "lib_mess.h"

void AddNameList(NameList *src, char *name){
  if(src->len < MAX_NAMELIST_SIZE){
    strcpy(src->name[src->len], name);
    src->len++;
  } else {
    for(int i = 0; i < src->len - 1; i++){
      strcpy(src->name[i], src->name[i + 1]);
    }
    strcpy(src->name[src->len - 1], name);
  }
}

void DelNameList(NameList *src, char *name){
  for(int i = 0; i < src->len; i++){
    if(strcmp(src->name[i], name) == 0){
      for(int j = i; j <= src->len - 1; j++){
        strcpy(src->name[j], src->name[j + 1]);
      }
      src->len--;
      break;
    }
  }
}