#include <stdlib.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <semaphore.h>

#include "thread.h"

extern pthread_mutex_t mutex;

void *ThreadSendServer(void *arg){
  char *name_user = (char*)arg;
  mode_t mode_open = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
  int fd = shm_open(NAME_SHARE_MEMORY, O_RDWR, mode_open);
  if(fd == -1) {
    perror("shm_open");
    exit(1);
  }
  ftruncate(fd, sizeof(Controller));
  Controller *ctl = (Controller*)mmap(NULL, sizeof(Controller), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  sem_t *sem = sem_open(NAME_SEMAPHORE, O_RDWR, mode_open, 1);
  NameList *list = &ctl->list;
  MessageStorage *storage = &ctl->storage;
  int x, y;
  getmaxyx(stdscr, y, x);
  WINDOW *wnd = newwin(y / 4, x, (y / 4) * 3, 0);
  Message msg = {0};
  strcpy(msg.name, name_user);
  while (1)
  {
    InputMessageWindow(wnd, &msg);
    if (!strcmp(msg.text, "/exit")) { 
      ctl->stop_client = 0;  
      strcpy(msg.name, "server");
      sprintf(msg.text, "user is out: %s", name_user);
      sem_wait(sem);
      AddStorageMessege(&ctl->storage, &msg);
      DelNameList(list, name_user);
      sem_post(sem);
      break;
    }
    sem_wait(sem);
    AddStorageMessege(&ctl->storage, &msg);
    sem_post(sem);
    usleep(1000);
  } 
  delwin(wnd);
  sem_close(sem);
  munmap(ctl, sizeof(Controller));
  return NULL;   
}
  