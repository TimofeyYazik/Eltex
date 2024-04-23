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

    mqd_t mqdes = mq_open(path, O_CREAT | O_RDONLY, 0666, NULL);
    if (mqdes == -1) {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    // Получаем сообщение из очереди
    ssize_t bytes_read = mq_receive(mqdes, message, 20, NULL);
    if (bytes_read == -1) {
        perror("mq_receive");
        exit(EXIT_FAILURE);
    }

    // Выводим полученное сообщение
    printf("%s\n", message);

    // Закрываем очередь
    mq_close(mqdes);

    // Удаляем очередь
    mq_unlink(path);

    exit(EXIT_SUCCESS);
}

