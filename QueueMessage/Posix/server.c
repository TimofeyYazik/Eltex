#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


char path[256] = "/Users/user/Eltex/QueueMessage/Posix";
char path_client[256] = "/Users/user/Eltex/QueueMessage/Posix/client";

int main(){
  char message[20] = "hello";
  int priority = 1;
  struct timespec delay = {1, 0};
  mqd_t mqdes_cl = mq_open(path_client, O_CREAT | O_RDONLY);
  mqd_t mqdes = mq_open(path, O_CREAT | O_WRONLY);
  mq_send(mqdes, message, 20, 1);
  mq_timedreceive(mqdes_cl, message, 20, &priority, &delay);
  printf("%s", message);
  mq_close(mqdes);
  mq_close(mqdes_cl);
  exit(EXIT_SUCCESS);
}