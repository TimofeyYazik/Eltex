#include "lib_mess.h"

void ShiftDsList(DsList *ds_list, int index){
  mq_close(ds_list->ds[index]);
  for(int i = index; i < ds_list->len - 1; i++){
    ds_list->ds[i] = ds_list->ds[i + 1];
  }
  ds_list->len--;
}
