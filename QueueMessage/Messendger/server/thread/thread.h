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

void MsgCopy(Message *dst, Message *src);
void *ThreadStop(void *arg);
void *ThreadSendClient(void *arg);
void *ThreadReceiveClient(void *arg);
void *ThreadRegisterClient(void *arg);

#endif //THREAD_H_