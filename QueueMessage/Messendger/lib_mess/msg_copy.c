#include "lib_mess.h"

void MsgCopy(Message *dst, Message *src){
  dst->status = src->status;
  strcpy(dst->name, src->name);
  strcpy(dst->text, src->text);
}