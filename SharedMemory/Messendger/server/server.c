#include <stdlib.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/mman.h>

#include "../custom_type.h"

MessageStorage storage = {0};
// volatile int stop_server = 1;

int main(){
  MessageStorage storage = {0};
  int fd = shm_open(NAME_SHARE_MEMORY, O_CREAT | O_RDWR, S_IWUSR | S_IRUSR);
  int stop = 1;
  ftruncate(fd, SIZE_MEMORY);
  MessageStorage *handler_ptr = (MessageStorage*)mmap(NULL, SIZE_MEMORY, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  handler_ptr->len = 0;
  handler_ptr->size = 10;
  handler_ptr->msg = (Message*)malloc(sizeof(Message) * handler_ptr->size);
  strcpy(handler_ptr->msg[0].name, "name");
  strcpy(handler_ptr->msg[0].text, "text");
  while (stop)
  {
    scanf("%d", &stop);
  }
  
  shm_unlink(NAME_SHARE_MEMORY);
  exit(EXIT_SUCCESS);
}