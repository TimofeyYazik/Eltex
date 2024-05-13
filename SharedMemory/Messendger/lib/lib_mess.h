#ifndef LIB_MESS_H_
#define LIB_MESS_H_

#include "../config.h"
#include <string.h>

void MsgCopy(Message *dst, Message *src);
void AddStorageMessege(MessageStorage *src, Message *msg);
void AddNameList(NameList *src, char *name);
void DelNameList(NameList *src, char *name);

#endif // LIB_MESS_H_