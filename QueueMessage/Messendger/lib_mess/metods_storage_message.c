#include "lib_mess.h"

void AddStorageMessege(MessageStorage *dst, Message *src){
  MsgCopy(&dst->msg[dst->len], src);
  src->len++;
  if(src->len == src->size) StorageMemRealloc(dst);
}