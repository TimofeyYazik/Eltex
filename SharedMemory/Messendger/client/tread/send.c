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
  int x, y;
  getmaxyx(stdscr, y, x);
  WINDOW *wnd = newwin(y / 4, x, (y / 4) * 3, 0);
  Message msg = {0};
  while (1)
  {
    InputMessageWindow(wnd, &msg);
    if (!strcmp(msg.text, "/exit")) {
      strcpy(msg.name , "server");
      sprintf(msg.text, "client is out: %s", cont->name_user);
      AddStorageMessege(cont->storage, &msg);
      DelNameList(cont->list, cont->name_user);
      cont->stop_client = 0;  
      break;
    }
    sem_wait(cont->sem);
    AddStorageMessege(cont->storage, &msg);
    sem_post(cont->sem);
  } 
  return NULL;   
}
  
