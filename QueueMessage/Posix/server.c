// #include <fcntl.h>
// #include <sys/stat.h>
// #include <mqueue.h>
// #include <stdlib.h>
// #include <stdio.h>
// #include <time.h>
// #include <unistd.h>
// #include <errno.h>

// #define MAX_PATH_LENGTH 256

// typedef struct{
//   char text[20];
// } Message;

// int main() {
//     char path[MAX_PATH_LENGTH] = "/home/dinahumb/Eltex/QueueMessage/Posix";
//     Message msg;
//     sprintf(msg.text, "hello");
//     struct mq_attr attr;                  
//     attr.mq_flags = 0;            
//     attr.mq_maxmsg = 50; 
//     attr.mq_msgsize = sizeof(msg);
//     attr.mq_curmsgs = 0;         

//     mqd_t mqdes = mq_open(path, O_CREAT | O_WRONLY, S_IWUSR | S_IRUSR, &attr);
//     if (mqdes == -1) {
//       fprintf(stderr, "mq_open failed with error: %d\n", errno);
//       perror("mq_open");
//       mq_close(mqdes);
//       mq_unlink(path);
//       exit(EXIT_FAILURE);
//     }

//     mq_send(mqdes, (char *)&msg, sizeof(Message), 1);
//     printf("sent: %s\n", msg.text);
//     printf("Press enter to exit\n");
//     getchar();
//     mq_close(mqdes);
//     mq_unlink(path);
//     exit(EXIT_SUCCESS);
// }

#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_PATH_LENGTH 256

int main() {
    char path[MAX_PATH_LENGTH] = "/my_message_queue"; // Путь к файлу очереди сообщений

    struct mq_attr attr; // Структура для атрибутов очереди
    attr.mq_flags = 0; // Флаги (нет специальных флагов)
    attr.mq_maxmsg = 10; // Максимальное количество сообщений в очереди
    attr.mq_msgsize = 1024; // Максимальный размер каждого сообщения в очереди
    attr.mq_curmsgs = 0; // Текущее количество сообщений в очереди

    mqd_t mqdes = mq_open(path, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, &attr); // Создание файла очереди сообщений
    if (mqdes == (mqd_t)-1) { // Проверка на успешное создание
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    mq_close(mqdes); // Закрыть дескриптор файла очереди сообщений

    printf("Message queue file created successfully.\n");

    exit(EXIT_SUCCESS);
}
