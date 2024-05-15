#include <stdlib.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
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
  refresh();
  while (ctl->stop_client) {
    perror("HUUUUUUUUUY\n");
    MessageWindow(wnd_msg, storage, (y / 4) * 3);
    UserWindow(wnd_list, list);
  }
  sem_close(ctl->sem);
  munmap(ctl, sizeof(Controller));
  return NULL;
}