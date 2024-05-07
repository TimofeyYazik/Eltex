#include "lib_mess.h"

void DsListMemRealloc(DsList *ds_list){
  ds_list->size = ds_list->size * 2 - (ds_list->size / 2);
  ds_list->ds = realloc(ds_list->ds, sizeof(mqd_t) * ds_list->size);
}