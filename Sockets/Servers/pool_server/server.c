#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#define POOL_THREADS 20
#define SA struct sockaddr
#define PORT 6666
#define SIZE_BUFF 80
#define IP_ADDRESS "127.0.0.2"
#define handler_error(text) \
    do { \
        perror(text); \
        exit(EXIT_FAILURE); \
    } while (0);

typedef struct {
    int port;
    int busy;
} ServerInfo;

volatile int stop = 1;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
ServerInfo serv[POOL_THREADS] = {0};

void *ChildServer(void *port_p) {
    int *port = (int *)port_p;
    time_t time_now;
    char buff[SIZE_BUFF] = {0};
    int ip_address = 0;
    struct sockaddr_in server_settings, client_settings;
    socklen_t client_size = sizeof(client_settings);

    inet_pton(AF_INET, IP_ADDRESS, &ip_address);
    int thread_sfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (thread_sfd == -1) {
        handler_error("socket");
    }

    server_settings.sin_family = AF_INET;
    server_settings.sin_addr.s_addr = ip_address;
    server_settings.sin_port = htons(*port);

    pthread_mutex_lock(&mutex);
    while (1) {
        if (bind(thread_sfd, (SA *)&server_settings, sizeof(server_settings)) == -1) {
            if (errno == EADDRINUSE) {
                (*port)++;
                server_settings.sin_port = htons(*port);
                continue;
            } else {
                handler_error("bind");
            }
        }
        break;
    }

    int serv_num = 0;
    for (int i = 0; i < POOL_THREADS; i++) {
        if (serv[i].port == 0) {
            serv[i].port = *port;
            serv_num = i;
            break;
        }
    }
    pthread_mutex_unlock(&mutex);

    while (stop) {
        ssize_t recv_bytes = recvfrom(thread_sfd, buff, SIZE_BUFF, 0, (SA *)&client_settings, &client_size);
        if (recv_bytes == -1) {
            perror("recvfrom");
            continue;
        }

        printf("RECEIVED from client %s\n", buff);

        if (!strcmp(buff, "exit")) {
            serv[serv_num].busy = 0;
            ssize_t sent_bytes = sendto(thread_sfd, buff, SIZE_BUFF, 0, (SA *)&client_settings, client_size);
            if (sent_bytes == -1) {
                perror("sendto");
            }
            printf("STOPPED SERVING CLIENT\n");
            break;
        } else {
            time(&time_now);
            strncpy(buff, ctime(&time_now), SIZE_BUFF-1);
            ssize_t sent_bytes = sendto(thread_sfd, buff, SIZE_BUFF, 0, (SA *)&client_settings, client_size);
            if (sent_bytes == -1) {
                perror("sendto");
            }
            printf("SENT to client %s\n", buff);
        }
    }

    close(thread_sfd);
    printf("THREAD IS EXITING\n");
    return NULL;
}

void *StopServer(void *ip) {
    int *ip_address = (int *)ip;
    while (stop) {
        if (scanf("%d", &stop) != 1) {
            stop = 0;
        }
    }

    int main_sfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (main_sfd == -1) {
        handler_error("socket");
    }

    struct sockaddr_in server_settings;
    server_settings.sin_family = AF_INET;
    server_settings.sin_addr.s_addr = *ip_address;
    server_settings.sin_port = htons(PORT);

    char buff[SIZE_BUFF] = "close";
    ssize_t sent_bytes = sendto(main_sfd, buff, SIZE_BUFF, 0, (SA *)&server_settings, sizeof(server_settings));
    if (sent_bytes == -1) {
        perror("sendto");
    }

    return NULL;
}

int main() {
    pthread_t threads[POOL_THREADS] = {0};
    int ip_address = 0;
    inet_pton(AF_INET, IP_ADDRESS, &ip_address);

    int main_sfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (main_sfd == -1) {
        handler_error("socket");
    }

    struct sockaddr_in server_settings;
    server_settings.sin_family = AF_INET;
    server_settings.sin_addr.s_addr = ip_address;
    server_settings.sin_port = htons(PORT);

    if (bind(main_sfd, (SA *)&server_settings, sizeof(server_settings)) == -1) {
        handler_error("bind");
    }

    pthread_t stop_thread;
    pthread_create(&stop_thread, NULL, StopServer, (void *)&ip_address);

    int ports[POOL_THREADS] = {0};
    for (int i = 0; i < POOL_THREADS; i++) {
        ports[i] = PORT + i + 1;
        pthread_create(&threads[i], NULL, ChildServer, &ports[i]);
    }

    printf("SERVER STARTED\n");
    printf("PRESS 0 TO STOP SERVER\n");

    char buff[SIZE_BUFF] = {0};
    while (stop) {
        struct sockaddr_in client_settings;
        socklen_t client_size = sizeof(client_settings);
        ssize_t recv_bytes = recvfrom(main_sfd, buff, SIZE_BUFF, 0, (SA *)&client_settings, &client_size);
        if (recv_bytes == -1) {
            perror("recvfrom");
            continue;
        }

        if (!strcmp(buff, "close")) {
            break;
        }

        if (strcmp(buff, "conn")) {
            continue;
        }

        int i;
        for (i = 0; i < POOL_THREADS; i++) {
            if (serv[i].busy == 0 && serv[i].port != 0) {
                serv[i].busy = 1;
                break;
            }
        }

        snprintf(buff, SIZE_BUFF, "%d", serv[i].port);
        ssize_t sent_bytes = sendto(main_sfd, buff, SIZE_BUFF, 0, (SA *)&client_settings, client_size);
        if (sent_bytes == -1) {
            perror("sendto");
        }
    }

    pthread_join(stop_thread, NULL);
    for (int i = 0; i < POOL_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    close(main_sfd);
    printf("SERVER STOPPED\n");
    return EXIT_SUCCESS;
}
