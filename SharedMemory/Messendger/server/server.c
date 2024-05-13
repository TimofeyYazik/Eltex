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
  sem_t *sem = sem_open(NAME_SEMAPHORE, O_CREAT | O_RDWR, mode_mqueue, 1);
  sem_post(sem);
  int fd = shm_open(NAME_SHARE_MEMORY, O_CREAT | O_RDWR, mode_mqueue);
  ftruncate(fd, sizeof(Controller));
  if(fd == -1) {
    perror("shm_open");
    exit(1);
  }
  Controller *cont_ptr = (Controller*)mmap(NULL, sizeof(Controller), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  cont_ptr->list.len = 0;
  cont_ptr->storage.len = 0;
  printf("Enter 0 to stop server\n");
  printf("%d %d\n", cont_ptr->list.len, cont_ptr->storage.len);
  int stop_server = 1;
  while (1)
  {
    scanf("%d", &stop_server);
    printf("%s\n", cont_ptr->list.name[0]);
    if(stop_server == 0) break;
  }
  munmap(cont_ptr, sizeof(Controller));
  shm_unlink(NAME_SHARE_MEMORY);
  sem_close(sem);
  sem_unlink(NAME_SEMAPHORE);
  exit(EXIT_SUCCESS);
}