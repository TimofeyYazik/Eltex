#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define MAX_PATH_LENGTH 256

int main() {
    char path[MAX_PATH_LENGTH] = "/server";
    char message[20] = "hello";

    mqd_t mqdes = mq_open(path, O_CREAT | O_WRONLY, 0666, NULL);
    if (mqdes == -1) {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    // Отправляем сообщение в очередь
    mq_send(mqdes, message, 6, 10);

    // Закрываем очередь
    mq_close(mqdes);

    exit(EXIT_SUCCESS);
}

