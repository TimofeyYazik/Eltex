#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define MAX_PATH_LENGTH 256

int main() {
    char path[MAX_PATH_LENGTH] = "/home/dinahumb/Eltex/QueueMessage/Posix";
    char message[10] = "hello";
    struct mq_attr attr;                  
    attr.mq_flags = 0;            
    attr.mq_maxmsg = 50; 
    attr.mq_msgsize = 30;
    attr.mq_curmsgs = 0;         

    mqd_t mqdes = mq_open(path, O_CREAT | O_WRONLY, 0666, &attr);
    if (mqdes == -1) {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    mq_send(mqdes, message, 10, 10);

    mq_close(mqdes);

    exit(EXIT_SUCCESS);
}

