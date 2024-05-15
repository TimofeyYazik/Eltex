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

extern pthread_mutex_t mutex;

void *ThreadSendServer(void *arg){
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
  int x, y;
  getmaxyx(stdscr, y, x);
  WINDOW *wnd = newwin(y / 4, x, (y / 4) * 3, 0);
  Message msg = {0};
  while (1)
  {
    pthread_mutex_lock(&mutex);
    InputMessageWindow(wnd, &msg);
    pthread_mutex_unlock(&mutex);
    if (!strcmp(msg.text, "/exit")) {
      // strcpy(msg.name, "server");
      // sprinf(msg.text, "client is out: %s")
      ctl->stop_client = 0;  
      break;
    }
    sem_wait(ctl->sem);
    AddStorageMessege(&ctl->storage, &msg);
    sem_post(ctl->sem);
    usleep(1000);
  } 
  delwin(wnd);
  sem_close(ctl->sem);
  munmap(ctl, sizeof(Controller));
  return NULL;   
}
  
