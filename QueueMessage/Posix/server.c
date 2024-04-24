#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
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
    int priority = 0;
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
      mq_unlink(MQ_NAME);
      exit(EXIT_FAILURE);
    }
    mq_send(mqdes, (char *)&msg, sizeof(Message), 1);
    printf("sent: %s\n", msg.text);
    printf("Press enter to request message\n");
    getchar();
    ssize_t bytes_read = mq_receive(mqdes_client, (char*)&msg, sizeof(Message), &priority);
    printf("req: %s prio: %d", msg.text, priority);
    mq_close(mqdes);
    mq_close(mqdes_client);
    mq_unlink(MQ_NAME);
    mq_unlink(MQ_NAME_CL);
    exit(EXIT_SUCCESS);
}

