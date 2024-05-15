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
  // int len_namelist = 0;
  // int len_storage = 0;
  int x, y;
  getmaxyx(stdscr, y, x);
  WINDOW *wnd_msg = newwin((y / 4) * 3, (x / 4) * 3, 0, 0);
  box(wnd_msg, 0, 0);
  WINDOW *wnd_list = newwin((y / 4) * 3, (x / 4), 0, (x / 4) * 3);
  box(wnd_list, 0, 0);
  while (ctl->stop_client) {
    // sem_wait(ctl->sem);
    MessageWindow(wnd_msg, storage, (y / 4) * 3);
    UserWindow(wnd_list, list);
    // sem_post(ctl->sem);
    // if(len_namelist > list->len) {
    //   len_namelist = list->len;
    // }   
    // if(len_namelist < list->len) {
    //   UserWindow(wnd, list);
    //   len_namelist = list->len;
    // }
    // if(len_storage < storage->len) {
    //   MessageWindow(wnd, storage, (y / 4) * 3);
    //   len_storage = storage->len;
    // }
    usleep(10000);
  }
  sem_close(ctl->sem);
  munmap(ctl, sizeof(Controller));
  return NULL;
}