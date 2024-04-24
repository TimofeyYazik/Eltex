#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>

#define MAX_PATH_LENGTH 256

typedef struct{
  char text[20];
} Message;

int main() {
    char MQ_NAME[10] = "/my_queue_server";
    char MQ_NAME_CL[10] = "/my_queue_client";
    Message msg;
    sprintf(msg.text, "hello");
    struct mq_attr attr;                  
    attr.mq_flags = 0;            
    attr.mq_maxmsg = 50; 
    attr.mq_msgsize = sizeof(msg);
    attr.mq_curmsgs = 0;         

    mqd_t mqdes = mq_open(MQ_NAME, O_CREAT | O_WRONLY, S_IWUSR | S_IRUSR, &attr);
    mqd_t mqdes_client = mq_open(MQ_NAME_CL, O_CREAT | O_RDONLY,  S_IWUSR | S_IRUSR, &attr);
    if (mqdes == -1) {
      fprintf(stderr, "mq_open failed with error: %d\n", errno);
      perror("mq_open");
      mq_close(mqdes);
      mq_unlink(path);
      exit(EXIT_FAILURE);
    }
    ssize_t bytes_read = mq_receive(mqdes, (char*)&msg, sizeof(Message), NULL);
    if (bytes_read == -1) {
        perror("mq_receive");mq_close(mqdes);
        mq_unlink(path);
        exit(EXIT_FAILURE);
    }

    printf("%s\n", msg.text);
    strcpy(msg.text, "hi   ");
    mq_send(mqdes_client, (char*)&msg, sizeof(Message), 1);

    mq_close(mqdes);

    exit(EXIT_SUCCESS);
}
