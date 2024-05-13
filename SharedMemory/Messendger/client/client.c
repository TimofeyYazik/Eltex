#include <stdlib.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <semaphore.h>

#include "../config.h"
#include "ui/ui.h"
#include "tread/thread.h"


int main(){
  mode_t mode_open = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
  int fd = shm_open(NAME_SHARE_MEMORY, O_RDWR, mode_open);
  if(fd == -1) {
    perror("shm_open");
    exit(1);
  }
  ftruncate(fd, sizeof(Controller));
  Controller *cont = (Controller*)mmap(NULL, sizeof(Controller), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  cont->sem = sem_open(NAME_SEMAPHORE, O_RDWR, mode_open, 1);
  Register(cont);
  pthread_t thread_send;
  pthread_t thread_receive;  
  initscr();
  pthread_create(&thread_send, NULL, ThreadSendServer, (void*)&cont);
  pthread_create(&thread_receive, NULL, ThreadRecvServer, (void*)&cont);
  pthread_join(thread_send, NULL);
  pthread_join(thread_receive, NULL);
  endwin();
  munmap(cont, sizeof(Controller));
  sem_close(cont->sem);
  exit(EXIT_SUCCESS);
}
