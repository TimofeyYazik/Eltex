#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <pthread.h>

#define PORT 6666
#define IP_ADDRES "127.0.0.1"
#define SA struct sockaddr
#define SIZE_BUFF 120
#define handler_error(text) \
do{ perror(text); exit(EXIT_FAILURE); } while(1);

void *ThreadStop(void *stop_p){
    int *stop = stop_p;
    while (*stop) {
        if(scanf("%d", stop) != 1){
            *stop = 0;
        }
    }
    return NULL;
}

int main() {
    int sfd = 0;
    int stop = 1;
    char buff[SIZE_BUFF] = {0};
    struct sockaddr_in server_settings, client_endpoint;

    // Создаем UDP socket
    sfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sfd == -1) {
        handler_error("socket");
    }

    // Устанавливаем параметры сервера
    server_settings.sin_family = AF_INET;
    server_settings.sin_addr.s_addr = inet_addr(IP_ADDRES);
    server_settings.sin_port = htons(PORT);

    // Привязываем сокет
    if (bind(sfd, (SA*)&server_settings, sizeof(server_settings)) == -1) {
        handler_error("bind");
    }

    socklen_t size = sizeof(client_endpoint);
    pthread_t stop_client = 0;

    pthread_create(&stop_client, NULL, ThreadStop, &stop);

    printf("PRESS 0 (ZERO) CLIENT STOP\n");
    while (stop) {
        int recv_len = recvfrom(sfd, buff, SIZE_BUFF, 0, (SA*)&client_endpoint, &size);
        if (recv_len == -1) {
            handler_error("recvfrom");
        }

        printf("Получено сообщение: %s\n", buff);

        // Ответное сообщение
        strcpy(buff, "Ответ от сервера");

        // Отправка ответа
        if (sendto(sfd, buff, strlen(buff) + 1, 0, (SA*)&client_endpoint, size) == -1) {
            handler_error("sendto");
        }

        printf("Ответ отправлен!\n");
    }

    pthread_join(stop_client, NULL);
    close(sfd);
    return 0;
}

