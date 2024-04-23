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
    char message[20];

    mqd_t mqdes = mq_open(path, O_RDWR);
    if (mqdes == -1) {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    // Получаем сообщение из очереди
    unsigned int priority;
    mq_receive(mqdes, message, 20, &priority);

    // Закрываем очередь
    mq_close(mqdes);

    // Выводим полученное сообщение
    printf("Received message: %s\n", message);

    exit(EXIT_SUCCESS);
}
