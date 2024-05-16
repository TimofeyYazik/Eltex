#ifndef MESSENDGER_THREAD_H
#define MESSENDGER_THREAD_H

#include "../../config.h"
#include "../../lib/lib_mess.h"
#include "../ui/ui.h"
#include <stdlib.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <pthread.h>
#include <sys/mman.h>
#include <semaphore.h>

void *ThreadSendServer(void *arg);
void *ThreadRecvServer(void *arg);

#endif //MESSENDGER_THREAD_H