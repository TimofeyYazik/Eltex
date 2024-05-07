#include "thread.h"

void MsgCopy(Message *dst, Message *src){
  strcpy(dst->name, src->name);
  strcpy(dst->text, src->text);
}
void ShiftList(NameList *list, int index){
  for(int i = index; i < list->len - 1; i++){
    strcpy(list->name[i], list->name[i + 1]);
  }
  list->len--;
}

void StorageMemRealloc(MessageStorage *storage){
  storage->size = storage->size * 2 - (storage->size / 2);
  storage->msg = realloc(storage->msg, sizeof(Message) * storage->size);
}

void DsListMemRealloc(DsList *ds_list){
  ds_list->size = ds_list->size * 2 - (ds_list->size / 2);
  ds_list->ds = realloc(ds_list->ds, sizeof(mqd_t) * ds_list->size);
}

void ListMemRealloc(NameList *list){
  list->size = list->size * 2 - (list->size / 2);
  list->name = realloc(list->name, sizeof(char*) * list->size);
}

void ShiftDsList(DsList *ds_list, int index){
  mq_close(ds_list->ds[index]);
  for(int i = index; i < ds_list->len - 1; i++){
    ds_list->ds[i] = ds_list->ds[i + 1];
  }
  ds_list->len--;
}

void InitAttr(struct mq_attr *attr, size_t size){
  attr->mq_flags = 0;
  attr->mq_maxmsg = 50;
  attr->mq_msgsize = size;
  attr->mq_curmsgs = 0;
}