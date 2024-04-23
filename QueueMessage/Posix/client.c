#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


char path[256] = "/Users/user/Eltex/QueueMessage/Posix";

int main(){
  char message[20];
  mqd_t mqdes = mq_open(path, O_CREAT | O_WRONLY | O_RDONLY);
  mq_receive(mqdes, message, 20, 1);
  printf("%s\n", message);
  strcpy(message, "hi   ");
  mq_send(mqdes, message, 20, 1);
  exit(EXIT_SUCCESS);
}