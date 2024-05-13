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
  Controller *cont = (Controller*)arg;
  NameList *list = &cont->list;
  MessageStorage *storage = &cont->storage;
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
    sem_wait(cont->sem);
    AddStorageMessege(storage, &msg);
    sem_post(cont->sem);
  } 
  return NULL;   
}
  
