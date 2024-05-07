#ifndef THREAD_H_
#define THREAD_H_

#include <curses.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <mqueue.h>
#include <unistd.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>

#include "../ui/ui.h"
#include "../../custom_type.h"

void ShiftList(NameList *list, int index);
void InitAttr(struct mq_attr *attr, size_t size);
void MsgCopy(Message *dst, Message *src);
void *ThreadSendServer(void *arg);
void *ThreadReceiveServer(void *arg);
void *ThreadUserWindow(void *arg);


#endif //THREAD_H_