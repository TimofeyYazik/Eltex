#include "lib_mess.h"

void MsgCopy(Message *dst, Message *src){
  strcpy(dst->name, src->name);
  strcpy(dst->text, src->text);
}

void AddStorageMessege(MessageStorage *src, Message *msg){
  if(src->len < MAX_MESSAGE_SIZE){
    MsgCopy(&src->msg[src->len], msg);
    src->len++;
  } else {
    for(int i = 0; i < src->len - 2; i++){
      MsgCopy(&src->msg[i], &src->msg[i + 2]);
    }
    MsgCopy(&src->msg[src->len - 2], msg);
    src->len--;
  }
}


