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
#include "thread/thread.h"

char name_user[MAX_NAME_LEN];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int stop_client;

int main(){
  stop_client = 1;
  mode_t mode_open = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
  int fd = shm_open(NAME_SHARE_MEMORY, O_RDWR, mode_open);
  if(fd == -1) {
    perror("shm_open");
    exit(1);
  }
  ftruncate(fd, sizeof(Controller));
  Controller *ctl = (Controller*)mmap(NULL, sizeof(Controller), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  
  Register(ctl);
  pthread_t thread_send;
  pthread_t thread_receive;  
  initscr();
  int error = pthread_create(&thread_receive, NULL, ThreadRecvServer, (void*)ctl);
  if(error != 0) {
    perror("pthread_create");
    exit(1);
  }
  error = pthread_create(&thread_send, NULL, ThreadSendServer, (void*)ctl); 
  if(error != 0) {
    perror("pthread_create");
    exit(1);
  }
  pthread_join(thread_send, NULL);
  pthread_join(thread_receive, NULL);
  endwin();
  munmap(ctl, sizeof(Controller));
  pthread_mutex_destroy(&mutex);
  exit(EXIT_SUCCESS);
}
