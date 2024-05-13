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

void *ThreadRecvServer(void *arg){
  Controller *cont = (Controller*)arg;
  NameList *list = &cont->list;
  MessageStorage *storage = storage;
  int len_namelist = 0;
  int len_storage = 0;
  int x, y;
  getmaxyx(stdscr, y, x);
  WINDOW *wnd = newwin(y / 4, x, (y / 4) * 3, 0);
  while (cont->stop_client) {
    sem_wait(cont->sem);
    if(len_namelist > list->len) {
      len_namelist = list->len;
    }   
    if(len_namelist < list->len) {
      UserWindow(wnd, list);
      len_namelist = list->len;
    }
    if(len_storage < storage->len) {
      MessageWindow(wnd, storage, (y / 4) * 3);
      len_storage = storage->len;
    }
    sem_post(cont->sem);
    wrefresh(wnd);
    usleep(10000);
  }
  return NULL;
}