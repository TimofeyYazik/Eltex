#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>


#define MAX_PATH_LENGTH 256

int main() {
  char path[MAX_PATH_LENGTH] = "/server";
  char path_client[MAX_PATH_LENGTH] = "/client";
  int priority = 0;
  char message[20];
  // mqd_t mqdes_cl = mq_open(path_client, O_CREAT | O_WRONLY);
  mqd_t mqdes = mq_open(path, O_CREAT | O_RDONLY, 0666, NULL);
  mq_receive(mqdes, message, 20, &priority);
  sleep(1);
  printf("%s\n", message);
  // strcpy(message, "hi  ");
  // mq_send(mqdes_cl, message, 20, 1);
  mq_close(mqdes);
  // mq_close(mqdes_cl);
  exit(EXIT_SUCCESS);
}