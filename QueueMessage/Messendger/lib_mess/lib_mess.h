#ifndef LIB_MESS_H_
#define LIB_MESS_H_

#include "../custom_type.h"

void ShiftList(NameList *list, int index);
void InitAttr(struct mq_attr *attr, size_t size);
void MsgCopy(Message *dst, Message *src);
void StorageMemRealloc(MessageStorage *src);
void DsListMemRealloc(DsList *src);
void ListMemRealloc(NameList *src);
void ShiftDsList(DsList *ds_list, int index);

#endif // LIB_MESS_H_