#ifndef THREAD_H_
#define THREAD_H_

#include <curses.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <mqueue.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>
#include <signal.h>
#include <termios.h>

#include "../../lib_mess/lib_mess.h"
#include "../ui/ui.h"
#include "../../custom_type.h"

void *ThreadStop(void *arg);
void *ThreadSendServer(void *arg);
void *ThreadReceiveServer(void *arg);


#endif //THREAD_H_