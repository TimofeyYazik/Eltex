#include "thread.h"

void InitAttr(struct mq_attr *attr, size_t size){
  attr->mq_flags = 0;
  attr->mq_maxmsg = 50;
  attr->mq_msgsize = size;
  attr->mq_curmsgs = 0;
}

void MsgCopy(Message *dst, Message *src){
  strcpy(dst->name, src->name);
  strcpy(dst->text, src->text);
}

void ShiftList(NameList *list, int index){
  for(int i = index; i < list->len - 1; i++){
    strcpy(list->name[i], list->name[i + 1]);
  }
  list->len--;
  free(list->name[list->len]);
}