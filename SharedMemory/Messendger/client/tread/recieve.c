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
  Controller *cont = (Controller*)mmap(NULL, sizeof(Controller), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  NameList *list = &cont->list;
  MessageStorage *storage = &cont->storage;
  int len_namelist = 0;
  int len_storage = 0;
  int x, y;
  getmaxyx(stdscr, y, x);
  WINDOW *wnd = newwin(y / 4, x, (y / 4) * 3, 0);
  while (cont->stop_client) {
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
    wrefresh(wnd);
    usleep(10000);
  }
  return NULL;
}