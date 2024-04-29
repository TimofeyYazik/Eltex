#include <stdlib.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>

#include "config.h"

int main() {
  int fd = shm_open(PATH, O_CREAT | O_RDWR, S_IWUSR | S_IRUSR);
  ftruncate(fd, SIZE_MESSAGE);
  char *str = (char*)mmap(NULL, SIZE_MESSAGE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);  
  printf("%s", str);
  strcpy(str, "exit");
  munmap(str, SIZE_MESSAGE);
  exit(EXIT_SUCCESS);
}