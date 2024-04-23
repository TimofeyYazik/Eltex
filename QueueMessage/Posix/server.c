#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


#define MAX_PATH_LENGTH 256

int main() {
  char path[MAX_PATH_LENGTH];
  char path_client[MAX_PATH_LENGTH];

  sprintf(path, "/Users/user/Eltex/QueueMessage/Posix/server_%d", getpid());
  sprintf(path_client, "/Users/user/Eltex/QueueMessage/Posix/client_%d", getpid());
  char message[20] = "hello";
  int priority = 1;
  mqd_t mqdes_cl = mq_open(path_client, O_CREAT | O_RDONLY);
  mqd_t mqdes = mq_open(path, O_CREAT | O_WRONLY);
  mq_send(mqdes, message, 20, 1);
  mq_receive(mqdes_cl, message, 20, &priority);
  printf("%s", message);
  mq_close(mqdes);
  mq_close(mqdes_cl);
  exit(EXIT_SUCCESS);
}