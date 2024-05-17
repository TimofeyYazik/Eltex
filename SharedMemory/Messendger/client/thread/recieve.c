#include "thread.h"

extern pthread_mutex_t mutex;

void *ThreadRecvServer(void *arg){
  Controller *ctl = (Controller*)arg;
  NameList *list = &ctl->list;
  MessageStorage *storage = &ctl->storage;
  int len_namelist = 0;
  int len_storage = 0;
  int x, y;
  getmaxyx(stdscr, y, x);
  WINDOW *wnd = newwin((y / 4) * 3, (x / 4) * 3, 0, 0);
  box(wnd, 0, 0);
  WINDOW *wnd2 = newwin((y / 4) * 3, (x / 4), 0, (x / 4) * 3);
  box(wnd2, 0, 0);
  while (ctl->stop_client) { 
    if(len_namelist != list->len) {
      pthread_mutex_lock(&mutex);
      UserWindow(wnd2, list);
      pthread_mutex_unlock(&mutex);
      len_namelist = list->len;
    }
    if(len_storage != storage->len) {
      pthread_mutex_lock(&mutex);
      MessageWindow(wnd, storage, (y / 4) * 3);
      pthread_mutex_unlock(&mutex);
      len_storage = storage->len;
    }
  }
  delwin(wnd);
  delwin(wnd2);
  return NULL;
}