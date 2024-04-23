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
    char path[MAX_PATH_LENGTH] = "/home/dinahumb/Eltex/QueueMessage/Posix";
    char message[10] = {0};

    mqd_t mqdes = mq_open(path, O_RDONLY);
    if (mqdes == -1) {
        perror("mq_open");
        mq_close(mqdes);
        mq_unlink(path);
        exit(EXIT_FAILURE);
    }

    ssize_t bytes_read = mq_receive(mqdes, message, 10, NULL);
    if (bytes_read == -1) {
        perror("mq_receive");mq_close(mqdes);
        mq_unlink(path);
        exit(EXIT_FAILURE);
    }

    printf("%s\n", message);

    mq_close(mqdes);
    mq_unlink(path);

    exit(EXIT_SUCCESS);
}
