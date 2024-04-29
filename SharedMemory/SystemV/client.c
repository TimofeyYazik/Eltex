#include <stdlib.h>
#include <sys/shm.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include "config.h"

int main() {
  key_t key = ftok(PATH, 0);
  int shmid = shmget(key, SIZE_MESSAGE, IPC_CREAT | S_IWUSR | S_IRUSR);
  char *str = (char*)shmat(shmid,(void*)0, 0);
  printf("%s", str);
  strcpy(str, "exit");
  exit(EXIT_SUCCESS);
}