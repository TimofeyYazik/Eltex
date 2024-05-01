#ifndef THREAD_H_
#define THREAD_H_

#include <stdlib.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <pthread.h>

#include "../ui/ui.h"
#include "../../custom_type.h"

void MsgCopy(Message *dst, Message *src);
void *ThreadSendServer(void *arg);
void *ThreadReceiveServer(void *arg);
void *ThreadUserWindow(void *arg);


#endif //THREAD_H_