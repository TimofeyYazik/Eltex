#include <stdlib.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>

#include "config.h"

int main() {
  int fd = shm_open(PATH, O_CREAT | O_RDWR, S_IWUSR | S_IRUSR);
  ftruncate(fd, SIZE_MESSAGE);
  char *str = (char*)mmap(NULL, SIZE_MESSAGE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  strcpy(str, "hello\n");
  while(strcmp(str, "exit")) sleep(1);
  munmap(str, SIZE_MESSAGE);
  shm_unlink(PATH);
  exit(EXIT_SUCCESS);
}