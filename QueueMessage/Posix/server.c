#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
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

    mqd_t mqdes = mq_open(path, O_CREAT | O_WRONLY, S_IWUSR | S_IRUSR, &attr);
    if (mqdes == -1) {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    mq_send(mqdes, (char *)&msg, sizeof(Message), 1);
    printf("sent: %s\n", msg.text);
    printf("Press enter to exit\n");
    getchar();
    mq_close(mqdes);
    mq_unlink(path);
    exit(EXIT_SUCCESS);
}

