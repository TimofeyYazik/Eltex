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
  mode_t mode_open = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
  int fd = shm_open(NAME_SHARE_MEMORY, O_RDWR, mode_open);
  if(fd == -1) {
    perror("shm_open");
    exit(1);
  }
  ftruncate(fd, sizeof(Controller));
  Controller *ctl = (Controller*)mmap(NULL, sizeof(Controller), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  ctl->sem = sem_open(NAME_SEMAPHORE, O_RDWR, mode_open, 1);
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
    if(len_namelist > list->len) {
      len_namelist = list->len;
    }   
    if(len_namelist < list->len) {
      UserWindow(wnd2, list);
      len_namelist = list->len;
    }
    if(len_storage < storage->len) {
      MessageWindow(wnd, storage, (y / 4) * 3);
      len_storage = storage->len;
    }
    usleep(100000);
  }
  return NULL;
}