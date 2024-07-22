#include <malloc.h>
#include <stdint.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <sys/signal.h>

#define SA struct sockaddr
#define PORT 6667
#define LISTEN_USERS 8
#define SIZE_BUFF 80
#define IP_ADDRES "127.0.0.2"
#define handler_error(text) \
          do { perror(text); exit(EXIT_FAILURE); } while(0);

volatile int stop = 1;

typedef struct {
    int *arr;
    int len;
    int size;
} ActiveFD;

typedef struct {
    pthread_t *arr;
    int len;
    int size;
} Thread;

void *ChildServer(void *fd) {
    int *active_fd = (int *)fd;
    time_t time_now = 0;
    char buff[SIZE_BUFF] = {0};
    time(&time_now);
    strncpy(buff, ctime(&time_now), SIZE_BUFF-1);
    int send_bytes = send(*active_fd, buff, SIZE_BUFF, 0);
    if (send_bytes == -1) {
        perror("send");
    }
    close(*active_fd);
    return NULL;
}

static inline void FillSock(struct sockaddr_in *s){
  inet_pton(AF_INET, IP_ADDRES, &s->sin_addr.s_addr);
  s->sin_family = AF_INET;
  s->sin_port = htons(PORT);
}

void *StopServer(void *null) {
    while (stop) {
        if (scanf("%d", &stop) != 1) {
            stop = 0;
        }
    }
    int cfd = socket(AF_INET, SOCK_STREAM, 0);
    if (cfd == -1) {
        handler_error("socket");
    }
    struct sockaddr_in server_endpoint;
    FillSock(&server_endpoint);
    char buff[SIZE_BUFF] = {0};
    strcpy(buff, "close");
    if (connect(cfd, (SA*)&server_endpoint, sizeof(server_endpoint)) == -1) {
        handler_error("connect");
    }
    send(cfd, buff, SIZE_BUFF, 0);
    close(cfd);
    return NULL;
}


void AddFD(int fd, ActiveFD *obj) {
    if (obj->len == obj->size - 1) {
        obj->size = obj->size * 3 / 2;
        obj->arr = realloc(obj->arr, obj->size * sizeof(int));
        if (!obj->arr) {
            handler_error("realloc");
        }
    }
    obj->arr[obj->len] = fd;
    obj->len++;
}

int AddThread(int *fd, Thread *obj) {
    if (obj->len == obj->size) {
        obj->size = obj->size * 3 / 2;
        obj->arr = realloc(obj->arr, obj->size * sizeof(pthread_t));
        if (!obj->arr) {
            handler_error("realloc");
        }
    }
    if (pthread_create(&obj->arr[obj->len], NULL, ChildServer, (void *)fd) != 0) {
        perror("pthread_create");
        return 1;
    }
    obj->len++;
    return 0;
}

static inline void FillUserStruct(ActiveFD *a, Thread *t){
    a->size = 100;
    a->len = 0;
    a->arr = calloc(a->size, sizeof(int));
    if (!a->arr) {
        handler_error("calloc");
    }
    t->len = 0;
    t->size = 100;
    t->arr = calloc(t->size, sizeof(pthread_t));
    if (!t->arr) {
        handler_error("calloc");
    }
}

int main() {
    ActiveFD obj_act;
    Thread obj_thread;
    FillUserStruct(&obj_act, &obj_thread);
    int main_sfd = socket(AF_INET, SOCK_STREAM, 0);
    if (main_sfd == -1) {
        handler_error("socket");
    }
    struct sockaddr_in server_settings;
    FillSock(&server_settings);
    if (bind(main_sfd, (SA *)&server_settings, sizeof(server_settings)) == -1) {
        handler_error("bind");
    }
    if (listen(main_sfd, LISTEN_USERS) == -1) {
        handler_error("listen");
    }
    socklen_t len = sizeof(server_settings);
    pthread_t stop_thread;
    if (pthread_create(&stop_thread, NULL, StopServer, NULL) != 0) {
        handler_error("pthread_create");
    }

    printf("SERVER START WORK\n");
    printf("PRESS 0 (ZERO) TO STOP SERVER\n");
    char buff[SIZE_BUFF] = {0};
    while (stop) {
        int active_fd = accept(main_sfd, (SA *)&server_settings, &len);
        if (active_fd == -1) {
            perror("accept");
            continue;
        }
        int recv_bytes = recv(active_fd, buff, SIZE_BUFF, 0);
        if (recv_bytes == -1) {
            perror("recv");
            close(active_fd);
            continue;
        }
        if (strcmp(buff, "close") == 0) break;
        if (strcmp(buff, "conn")) {
            close(active_fd);
            continue;
        }
        printf("NEW CLIENT: %d\n", active_fd);
        AddFD(active_fd, &obj_act);
        if (AddThread(&obj_act.arr[obj_act.len - 1], &obj_thread) != 0) {
            strcpy(buff, "error");
            send(active_fd, buff, SIZE_BUFF, 0);
            obj_act.len--;
            close(obj_act.arr[obj_act.len]);
        }
    }

    pthread_join(stop_thread, NULL);
    for (int i = 0; i < obj_thread.len; i++) {
        pthread_join(obj_thread.arr[i], NULL);
    }
    close(main_sfd);
    free(obj_act.arr);
    free(obj_thread.arr);
    printf("SERVER END WORK\n");
    return EXIT_SUCCESS;
}
