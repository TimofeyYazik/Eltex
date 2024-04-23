#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define MAX_PATH_LENGTH 256

int main() {
    char path[MAX_PATH_LENGTH] = "/Users/user/Eltex/QueueMessage/Posix/server";
    char path_client[MAX_PATH_LENGTH] = "/Users/user/Eltex/QueueMessage/Posix/client";

    char message[20] = "hello";
    int priority = 0;
    mqd_t mqdes_cl = mq_open(path_client, O_CREAT | O_RDONLY, 0666, NULL);
    mqd_t mqdes = mq_open(path, O_CREAT | O_WRONLY, 0666, NULL);
    mq_send(mqdes, message, 20, 1);
    mq_receive(mqdes_cl, message, 20, &priority);
    printf("%s\n", message);
    mq_close(mqdes);
    mq_close(mqdes_cl);
    mq_unlink(path);
    mq_unlink(path_client);
    exit(EXIT_SUCCESS);
}
