#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

struct timespec {
  time_t tv_sec;        /* seconds */
  long   tv_nsec;       /* nanoseconds */
};

char path[256] = "/Users/user/Eltex/QueueMessage/Posix";

int main(){
  char message[20] = "hello";
  int priority = 1;
  struct timespec delay = {1, 0};
  mqt_t mqdes = mq_open(path, O_CREAT | O_WRONLY | O_RDONLY);
  mq_send(mqdes, &message, 20, 1);
  ssize_t k = 0;
  while (!k) {
    k = mq_timedreceive(mqdes, &message, 20, &priority, &delay);
  }
  printf("%s", message);
  mq_close(mqdes);
  exit(EXIT_SUCCESS);
}