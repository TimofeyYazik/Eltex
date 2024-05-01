#include <stdlib.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <semaphore.h>

#include "thread/thread.h"

char name[MAX_NAME_LEN] = {0};
int stop_client = 1;

int main(){
  int fd = shm_open(NAME_SHARE_MEMORY, O_CREAT | O_RDWR, S_IWUSR | S_IRUSR);
  ftruncate(fd, SIZE_MEMORY);
  if(fd == -1) {
    perror("shm_open");
    exit(1);
  }
  MessageStorage *handler_ptr = (MessageStorage*)mmap(NULL, SIZE_MEMORY, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  printf("%s %s\n", handler_ptr->msg[0].name, handler_ptr->msg[0].text);
  exit(EXIT_SUCCESS);
}

  // Register();
  // sem_t *sem = sem_open(NAME_SEMAPHORE, O_CREAT | O_RDWR, S_IWUSR | S_IRUSR, 1);
  // if(sem == SEM_FAILED) exit(EXIT_FAILURE);
  // // if(sem_getvalue(sem, NULL) == -1) exit(EXIT_FAILURE);
  // sem_post(sem);
  // pthread_t thread_send;
  // pthread_t thread_receive;

  // // pthread_create(&thread_send, NULL, ThreadSendServer, sem);
  // pthread_create(&thread_receive, NULL, ThreadReceiveServer, sem);

  // // pthread_join(thread_send, NULL);
  // pthread_join(thread_receive, NULL);