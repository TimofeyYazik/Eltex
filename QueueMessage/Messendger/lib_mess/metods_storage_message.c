#include "lib_mess.h"

void AddStorageMessege(MessageStorage *dst, Message *src){
  MsgCopy(&dst->msg[dst->len], src);
  dst->len++;
  if(dst->len == dst->size) StorageMemRealloc(dst);
}