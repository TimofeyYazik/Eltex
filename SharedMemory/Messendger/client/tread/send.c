#include <stdlib.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <semaphore.h>

#include "thread.h"


void *ThreadSendServer(void *arg){
  Controller *ctl = (Controller*)arg;
  NameList *list = &ctl->list;
  MessageStorage *storage = &ctl->storage;
  int x, y;
  getmaxyx(stdscr, y, x);
  WINDOW *wnd = newwin(y / 4, x, (y / 4) * 3, 0);
  Message msg = {0};
  while (1)
  {
    InputMessageWindow(wnd, &msg);
    // if (!strcmp(msg.text, "/exit")) {

    //   cont->stop_client = 0;  
    //   break;
    // }
    sem_wait(ctl->sem);
    AddStorageMessege(&ctl->storage, &msg);
    sem_post(ctl->sem);
  } 
  return NULL;   
}
  
