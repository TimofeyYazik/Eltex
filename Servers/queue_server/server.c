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

#define POOL_TREADS 30
#define SA struct sockaddr
#define PORT 6666
#define SIZE_BUFF 8
#define IP_ADDRES "127.0.0.1"
#define handler_error(text)                                                    \
  do {                                                                         \
    perror(text);                                                              \
    exit(EXIT_FAILURE);                                                        \
  } while (0);

ListServer *head = NULL;
volatile int stop = 1;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *ChildServer(void *null) {
  char time_buff[80] = {0};
  time_t time_now;
  char buff[SIZE_BUFF];
  ListServer *f = NULL;
  while (stop) {
    pthread_mutex_lock(&mutex);
    f = SearchFree(head);
    if (f == NULL) {
      sleep(1);
      pthread_mutex_unlock(&mutex);
      continue;
    }
    f->busy = 1;
    printf("CLIENT SERVED\n");
    pthread_mutex_unlock(&mutex);
    while (1) {
      recv(f->active_fd, buff, SIZE_BUFF, 0);
      printf("RECERV CLIENT: %d\n", f->active_fd);
      if (!strcmp(buff, "exit")) {
        send(f->active_fd, buff, SIZE_BUFF, 0);
        close(f->active_fd);
        printf("CLIENT IS OUT: %d\n", f->active_fd);
        Remove(f);
        break;
      } else {
        time(&time_now);
        strcpy(time_buff, ctime(&time_now));
        send(f->active_fd, (void *)time_buff, 80, 0);
        printf("SEND CLIENT: %d\n", f->active_fd);
      }
    }
  }
  printf("THREAD IS OUT\n");
  return NULL;
}

void *StopServer(void *s) {
  int *ip = s;
  int ip_addres = *ip;
  while (stop) {
    if (scanf("%d", &stop) != 1) {
      stop = 0;
    }
  }
  inet_pton(AF_INET, IP_ADDRES, &ip_addres);
  int cfd = socket(AF_INET, SOCK_STREAM, 0);
  if (cfd == -1) {
    handler_error("socket");
  }
  struct sockaddr_in server_connect;
  server_connect.sin_family = AF_INET;
  server_connect.sin_addr.s_addr = ip_addres;
  server_connect.sin_port = htons(PORT);
  char buff[SIZE_BUFF] = {0};
  strcpy(buff, "close");
  if (connect(cfd, (SA *)&server_connect, sizeof(server_connect)) == -1) {
    handler_error("ne vezet");
  }
  send(cfd, buff, SIZE_BUFF, 0);
  close(cfd);
  return NULL;
}

int main() {
  head = CreateList();
  pthread_t arr_treads[POOL_TREADS] = {0};
  int ip_addres = 0;
  inet_pton(AF_INET, IP_ADDRES, &ip_addres);
  int main_sfd = socket(AF_INET, SOCK_STREAM, 0);

  if (main_sfd == -1) {
    handler_error("socket");
  }

  struct sockaddr_in server_settings;
  server_settings.sin_family = AF_INET;
  server_settings.sin_addr.s_addr = ip_addres;
  server_settings.sin_port = htons(PORT);

  if (bind(main_sfd, (SA *)&server_settings, sizeof(server_settings)) == -1) {
    handler_error("bind");
  }
  if (listen(main_sfd, 100) == -1) {
    handler_error("listen");
  }

  socklen_t len = sizeof(server_settings);
  pthread_t stop_tread;
  pthread_create(&stop_tread, NULL, StopServer, (void *)&ip_addres);
  for (int i = 0; i < POOL_TREADS; i++) {
    pthread_create(&arr_treads[i], NULL, ChildServer, NULL);
  }

  printf("SERVER START WORK\n");
  printf("PRESS 0 (ZERO) SERVER STOP\n");

  char buff[8] = {0};
  while (stop) {
    int active_fd = accept(main_sfd, (SA *)&server_settings, &len);
    recv(active_fd, buff, 8, 0);
    if(!strcmp(buff, "close")) break;
    printf("NEW CLIENT: %d\n", active_fd);
    ListServer *new_client = malloc(sizeof(ListServer));
    new_client->active_fd = active_fd;
    InsertEnd(head, new_client);
  }
  pthread_join(stop_tread, NULL);
  for (int i = 0; i < POOL_TREADS; i++) {
    pthread_join(arr_treads[i], NULL);
  }
  close(main_sfd);
  free(head);
  printf("SERVER END WORK\n");
  exit(EXIT_SUCCESS);
}
