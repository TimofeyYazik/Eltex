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
    struct mq_attr attr;                  /* Атрибуты очереди */

    /* Установка атрибутов очереди */
    attr.mq_flags = 0;            /* Флаги атрибутов: 0 или O_NONBLOCK */
    attr.mq_maxmsg = 10; /* Максимальное количество сообщений в очереди */
    attr.mq_msgsize = 21; /* Максимальный размер сообщения */
    attr.mq_curmsgs = 0;           /* Текущее количество сообщений в очереди */


    mqd_t mqdes = mq_open(path, O_CREAT | O_WRONLY, 0666, &attr);
    if (mqdes == -1) {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    // Отправляем сообщение в очередь
    mq_send(mqdes, message, 20, 10);

    // Закрываем очередь
    mq_close(mqdes);

    exit(EXIT_SUCCESS);
}

