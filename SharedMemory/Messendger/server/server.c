#include <stdlib.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <stdio.h>

#include "../lib/lib_mess.h"
#include "../config.h"

int main(){
  mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
  int fd = shm_open(NAME_SHARE_MEMORY, O_CREAT | O_RDWR, mode);
  if(fd == -1) {
    perror("shm_open");
    exit(1);
  }
  ftruncate(fd, sizeof(Controller));
  if(fd == -1) {
    perror("shm_open");
    exit(1);
  }
  Controller *ctl = (Controller*)mmap(NULL, sizeof(Controller), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if(ctl == MAP_FAILED) {
    perror("mmap");
    exit(1);
  }
  MessageStorage *storage = &ctl->storage;
  NameList *list = &ctl->list;
  sem_t *sem = sem_open(NAME_SEMAPHORE, O_CREAT | O_RDWR, mode, 1);
  if(sem == SEM_FAILED) {
    perror("sem_open");
    munmap(ctl, sizeof(Controller));
    shm_unlink(NAME_SHARE_MEMORY);
    exit(1);
  }
  sem_post(sem);
  list->len = 0;
  storage->len = 0;
  printf("Enter 0 to stop server\n");
  int stop_server = 1;
  while (stop_server){ 
    if(scanf("%d", &stop_server) != 1) {
      printf("Enter 0 to stop server\n");
      continue;
    }
  }
  munmap(ctl, sizeof(Controller));
  shm_unlink(NAME_SHARE_MEMORY);
  sem_close(sem);
  sem_unlink(NAME_SEMAPHORE);
  exit(EXIT_SUCCESS);
}