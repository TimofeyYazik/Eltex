#ifndef THREAD_H_
#define THREAD_H_

#include <mqueue.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>

#include "../../custom_type.h"
#include "../../lib_mess/lib_mess.h"

void ShiftList(NameList *list, int index);
void InitAttr(struct mq_attr *attr, size_t size);
void MsgCopy(Message *dst, Message *src);
void StorageMemRealloc(MessageStorage *src);
void DsListMemRealloc(DsList *src);
void ListMemRealloc(NameList *src);
void ShiftDsList(DsList *ds_list, int index);

void *ThreadStop(void *arg);
void *ThreadSendClient(void *arg);
void *ThreadReceiveClient(void *arg);
void *ThreadRegisterClient(void *arg);

#endif //THREAD_H_