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
  mode_t mode_mqueue = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
  int fd = shm_open(NAME_SHARE_MEMORY, O_CREAT | O_RDWR, mode_mqueue);
  ftruncate(fd, sizeof(Controller));
  if(fd == -1) {
    perror("shm_open");
    exit(1);
  }
  Controller *ctl = (Controller*)mmap(NULL, sizeof(Controller), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  MessageStorage *storage = &ctl->storage;
  NameList *list = &ctl->list;
  ctl->sem = sem_open(NAME_SEMAPHORE, O_CREAT | O_RDWR, mode_mqueue, 1);
  sem_post(ctl->sem);
  list->len = 0;
  storage->len = 0;
  printf("Enter 0 to stop server\n");
  printf("%d %d\n", ctl->list.len, ctl->storage.len);
  int stop_server = 1;
  while (1)
  {
    scanf("%d", &stop_server);
    for(int i = 0; i < ctl->list.len;i++)
      printf("%s\n", ctl->list.name[i]);
    for(int i = 0; i < ctl->storage.len;i++)
      printf("%s\n", ctl->storage.msg[i].text);
    if(stop_server == 0) break;
  }
  munmap(ctl, sizeof(Controller));
  shm_unlink(NAME_SHARE_MEMORY);
  sem_unlink(NAME_SEMAPHORE);
  exit(EXIT_SUCCESS);
}