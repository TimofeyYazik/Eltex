#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_PATH_LENGTH 256

typedef struct{
  char text[20];
} Message;

int main() {
    char path[MAX_PATH_LENGTH] = "/indus";
    Message msg;
    sprintf(msg.text, "hello");
    struct mq_attr attr;                  
    attr.mq_flags = 0;            
    attr.mq_maxmsg = 50; 
    attr.mq_msgsize = sizeof(msg);
    attr.mq_curmsgs = 0;         

    mqd_t mqdes = mq_open(path, O_CREAT | O_RDONLY, S_IWUSR | S_IRUSR, &attr);
    if (mqdes == -1) {
        perror("mq_open");
        mq_close(mqdes);
        mq_unlink(path);
        exit(EXIT_FAILURE);
    }

    ssize_t bytes_read = mq_receive(mqdes, &msg, sizeof(Message), NULL);
    if (bytes_read == -1) {
        perror("mq_receive");mq_close(mqdes);
        mq_unlink(path);
        exit(EXIT_FAILURE);
    }

    printf("%s\n", msg.text);

    mq_close(mqdes);

    exit(EXIT_SUCCESS);
}
