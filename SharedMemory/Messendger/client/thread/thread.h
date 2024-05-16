#ifndef MESSENDGER_THREAD_H
#define MESSENDGER_THREAD_H

#include "../../config.h"
#include "../../lib/lib_mess.h"
#include "../ui/ui.h"

void *ThreadSendServer(void *arg);
void *ThreadRecvServer(void *arg);

#endif //MESSENDGER_THREAD_H