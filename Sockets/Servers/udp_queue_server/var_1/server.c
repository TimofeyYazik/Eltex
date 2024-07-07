#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/signal.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#include "double_list/list.h"

#define POOL_THREADS 20
#define SA struct sockaddr
#define PORT 6666
#define SIZE_BUFF 80
#define IP_ADDRESS "127.0.0.2"
#define handler_error(text) \
  do { perror(text); exit(EXIT_FAILURE); } while (0);

ListServer *head = NULL;
volatile int stop = 1;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static inline void CopySockaddr_in(struct sockaddr_in *dest, struct sockaddr_in *source);
void *StopServer(void *s);
static inline int SettingsChild(int *pt);


void *ChildServer(void *null) {
  struct sockaddr_in client;
  int port = 0;
  int fd = SettingsChild(&port);
  time_t time_now = 0;
  char buff[SIZE_BUFF] = {0};
  ListServer *f = NULL;
  while (stop) {
    pthread_mutex_lock(&mutex);
    f = SearchFree(head);
    if (f == NULL) {
      usleep(80);
      pthread_mutex_unlock(&mutex);
      continue;
    }
    CopySockaddr_in(&client, &f->sock);
    Remove(f);
    pthread_mutex_unlock(&mutex);
    sprintf(buff, "%d", port);
    sendto(fd, buff, SIZE_BUFF, 0, (SA*)&client, sizeof(f->sock));
    int recv_r = 0;
    int send_r = 0;
    socklen_t len = sizeof(f->sock);
    while (1) {
      if ((recv_r = recvfrom(fd, buff, SIZE_BUFF, 0, (SA*)&client, &len)) == -1) {
        perror("recv thread");
        break;
      }
      if (!strcmp(buff, "exit")) break;
      if (!strcmp(buff, "time")) {
        time(&time_now);
        strcpy(buff, ctime(&time_now));
        if ((send_r = sendto(fd, (void *)buff, SIZE_BUFF, 0, (SA*)&client, len)) == -1) {
          perror("send thread");
          break;
        }
      }
    }
  }
  return NULL;
}



int main() {
  head = CreateList();
  pthread_t arr_threads[POOL_THREADS] = {0};
  int main_sfd = socket(AF_INET, SOCK_DGRAM, 0);

  if (main_sfd == -1) {
    handler_error("socket");
  }

  struct sockaddr_in server_settings, client_endpoint;
  server_settings.sin_family = AF_INET;
  server_settings.sin_port = htons(PORT);
  inet_pton(AF_INET, IP_ADDRESS, &server_settings.sin_addr);
  socklen_t size = sizeof(client_endpoint);
  if (bind(main_sfd, (SA *)&server_settings, sizeof(server_settings)) == -1) {
    handler_error("bind");
  }

  socklen_t len = sizeof(server_settings);
  pthread_t stop_thread;
  pthread_create(&stop_thread, NULL, StopServer, NULL);
  for (int i = 0; i < POOL_THREADS; i++) {
    pthread_create(&arr_threads[i], NULL, ChildServer, NULL);
  }

  printf("SERVER START WORK\n");
  printf("PRESS 0 (ZERO) TO STOP SERVER\n");

  int recv_r = 0;
  char buff[SIZE_BUFF] = {0};
  while (stop) {
    if ((recv_r = recvfrom(main_sfd, buff, SIZE_BUFF, 0, (SA*)&client_endpoint, &size)) == -1) {
      perror("recv");
      continue;
    }
    if (!strcmp(buff, "close")) break;
    ListServer *new_client = malloc(sizeof(ListServer));
    CopySockaddr_in(&new_client->sock, &client_endpoint);
    pthread_mutex_lock(&mutex);
    InsertEnd(head, new_client);
    pthread_mutex_unlock(&mutex);
  }
  pthread_join(stop_thread, NULL);
  for (int i = 0; i < POOL_THREADS; i++) {
    pthread_join(arr_threads[i], NULL);
  }
  close(main_sfd);
  FreeList(head);
  printf("SERVER END WORK\n");
  exit(EXIT_SUCCESS);
}


static inline void CopySockaddr_in(struct sockaddr_in *dest, struct sockaddr_in *source) {
  dest->sin_addr = source->sin_addr;
  dest->sin_family = source->sin_family;
  dest->sin_port = source->sin_port;
}

void *StopServer(void *s) {
  while (stop) {
    if (scanf("%d", &stop) != 1) {
      stop = 0;
    }
  }
  int cfd = socket(AF_INET, SOCK_STREAM, 0);
  if (cfd == -1) {
    handler_error("socket");
  }
  struct sockaddr_in server_connect;
  memset(&server_connect, 0, sizeof(server_connect));
  server_connect.sin_family = AF_INET;
  server_connect.sin_port = htons(PORT);
  inet_pton(AF_INET, IP_ADDRESS, &server_connect.sin_addr);
  char buff[SIZE_BUFF] = {0};
  strcpy(buff, "close");
  sendto(cfd, buff, SIZE_BUFF, 0, (SA*)&server_connect, sizeof(server_connect));
  close(cfd);
  return NULL;
}

static inline int SettingsChild(int *pt){
  int port = PORT;
  int fd = socket(AF_INET, SOCK_DGRAM, 0);
  struct sockaddr_in server_settings, client_settings;
  server_settings.sin_family = AF_INET;
  inet_pton(AF_INET, IP_ADDRESS, &server_settings.sin_addr);
  server_settings.sin_port = htons(port);
  while (1) {
    if (bind(fd, (SA *)&server_settings, sizeof(server_settings)) == -1) {
      if (errno == EADDRINUSE) {
        port++;
        server_settings.sin_port = htons(port);
        continue;
      } else {
        handler_error("bind");
      }
    }
    break;
  }
  *pt = port;
  return fd;
}

