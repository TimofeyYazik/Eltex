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

#define POOL_TREADS 50
#define SA struct sockaddr
#define PORT 6666
#define SIZE_BUFF 80
#define IP_ADDRES "127.0.0.2"
#define handler_error(text)                                                    \
  do { perror(text); exit(EXIT_FAILURE); } while (0);

ListServer *head = NULL;
volatile int stop = 1;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *ChildServer(void *null) {
  int port = PORT;
  int fd = socket(AF_INET, SOCK_DGRAM, 0);
  struct sockaddr_in server_settings, client_settings;
  server_settings.sin_family = AF_INET;
  inet_pton(AF_INET, IP_ADDRES, &server_settings.sin_addr); 
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

  time_t time_now = 0;
  char buff[SIZE_BUFF] = {0};
  ListServer *f = NULL;
  while (stop) {
    pthread_mutex_lock(&mutex);
    f = SearchFree(head);
    if (f == NULL) {
      usleep(100);
      pthread_mutex_unlock(&mutex);
      continue;
    }
    f->busy = 1;
    pthread_mutex_unlock(&mutex);
    printf("CLIENT SERVED\n");
    sprintf(buff, "%d", port);
    sendto(fd, buff, SIZE_BUFF, 0, (SA*)&f->sock, sizeof(f->sock));
    int recv_r = 0;
    int send_r = 0;
    socklen_t len = sizeof(f->sock);
    while (1) {
      if((recv_r = recvfrom(fd, buff, SIZE_BUFF, 0, (SA*)&f->sock, &len) == -1)){
          perror("recv thread");
          Remove(f);
          break;
      } 
      printf("RECERV CLIENT\n");
      if (!strcmp(buff, "exit")) {
        printf("CLIENT IS OUT\n");
        Remove(f);
        break;
      } else if(!strcmp(buff, "time")){
        time(&time_now);
        strcpy(buff, ctime(&time_now));
        if((send_r = sendto(fd, (void *)buff, SIZE_BUFF, 0, (SA*)&f->sock, len)) == -1){
          perror("send thread");
          Remove(f);
          break;
        }
        printf("SEND CLIENT\n");
      }
    }
  }
  printf("THREAD IS OUT\n");
  return NULL;
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
  struct sockaddr_in server_connect, server_endpoint;
  memset(&server_endpoint, 0, sizeof(server_endpoint));
  server_connect.sin_family = AF_INET;
  server_connect.sin_port = htons(PORT);
  inet_pton(AF_INET, IP_ADDRES, &server_connect.sin_addr);
  char buff[SIZE_BUFF] = {0};
  strcpy(buff, "close");
  sendto(cfd, buff, SIZE_BUFF, 0, (SA*)&server_endpoint, sizeof(server_endpoint));
  close(cfd);
  return NULL;
}

void CopySockaddr_in(struct sockaddr_in *dest, struct sockaddr_in *source){
  dest->sin_addr = source->sin_addr;
  dest->sin_family = source->sin_family;
  dest->sin_port = source->sin_port;  
}

int main() {
  head = CreateList();
  pthread_t arr_treads[POOL_TREADS] = {0};
  int main_sfd = socket(AF_INET, SOCK_DGRAM, 0);

  if (main_sfd == -1) {
    handler_error("socket");
  }

  struct sockaddr_in server_settings, client_endpoint;
  server_settings.sin_family = AF_INET;
  server_settings.sin_port = htons(PORT);
  inet_pton(AF_INET, IP_ADDRES, &server_settings.sin_addr);
  socklen_t size = sizeof(client_endpoint); 
  if (bind(main_sfd, (SA *)&server_settings, sizeof(server_settings)) == -1) {
    handler_error("bind");
  }

  socklen_t len = sizeof(server_settings);
  pthread_t stop_tread;
  pthread_create(&stop_tread, NULL, StopServer, NULL);
  for (int i = 0; i < POOL_TREADS; i++) {
    pthread_create(&arr_treads[i], NULL, ChildServer, NULL);
  }

  printf("SERVER START WORK\n");
  printf("PRESS 0 (ZERO) SERVER STOP\n");

  int recv_r = 0;
  char buff[SIZE_BUFF] = {0};
  while (stop) {
    if((recv_r = recvfrom(main_sfd, buff, SIZE_BUFF, 0, (SA*)&client_endpoint, &size)) == -1){
      perror("recv");
      continue;
    }
    if(!strcmp(buff, "close")) break;
    ListServer *new_client = malloc(sizeof(ListServer));
    CopySockaddr_in(&new_client->sock, &client_endpoint);
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
