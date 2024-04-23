#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


char path[256] = "/Users/user/Eltex/QueueMessage/Posix/server";
char path_client[256] = "/Users/user/Eltex/QueueMessage/Posix/client";

int main(){
  int priority = 1;
  char message[20];
  mqd_t mqdes_cl = mq_open(path_client, O_CREAT | O_RDONLY);
  mqd_t mqdes = mq_open(path, O_CREAT | O_WRONLY);
  mq_receive(mqdes, message, 20, &priority);
  printf("%s\n", message);
  strcpy(message, "hi   ");
  mq_send(mqdes_cl, message, 20, 1);
  mq_close(mqdes);
  mq_close(mqdes_cl);
  exit(EXIT_SUCCESS);
}