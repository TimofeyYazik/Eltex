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
#define IP_ADDRES "127.0.0.2"
#define handler_error(text) \
  do { perror(text); exit(EXIT_FAILURE); } while (0);

ListServer *head = NULL;
volatile int stop = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//func
static inline void CopySockaddr_in(struct sockaddr_in *dest, struct sockaddr_in *source);
static inline int SettingsChild(int *pt);

void *ChildServer(void *null){
  int port = 0;
  int sock = SettingsChild(&port);
  ListServer *f;
  struct sockaddr_in client;
  char buff[SIZE_BUFF] = {0};
  while(!stop){
    pthread_mutex_lock(&mutex);
    f = SearchFree(head);
    if(f == NULL){
      pthread_mutex_unlock(&mutex);
      usleep(100);
      continue;
    }
    CopySockaddr_in(&client, &f->sock);
    Remove(f);
    pthread_mutex_unlock(&mutex);
    sendto(sock, buff, SIZE_BUFF, 0, (SA*)&client, sizeof(client));
  }
  
  close(sock);
  return NULL;
}


int main(){
  head = CreateList();
  pthread_t list[POOL_THREADS] = {0};
  pthread_t stop_thread = 0;
  for(int i = 0; i < POOL_THREADS; i++){
    pthread_create(&list[i], NULL, ChildServer, NULL);
  }
  
  int sock = socket(AF_INET, SOCK_DGRAM, 0);
  struct sockaddr_in serv, client;
  serv.sin_port = htons(PORT);
  serv.sin_family = AF_INET;
  char buff[SIZE_BUFF] = {0};
  inet_pton(sock, IP_ADDRES, &serv.sin_addr);
  socklen_t len = sizeof(client);
  while(!stop){
    recvfrom(sock, buff, SIZE_BUFF, 0, (SA*)&client, &len);
    ListServer *l = malloc(sizeof(ListServer));
    CopySockaddr_in(&l->sock, &client);
    InsertEnd(head, l);
  }
  FreeList(head);
  close(sock);
  exit(EXIT_SUCCESS);
}


static inline void CopySockaddr_in(struct sockaddr_in *dest, struct sockaddr_in *source) {
  dest->sin_addr = source->sin_addr;
  dest->sin_family = source->sin_family;
  dest->sin_port = source->sin_port;
}

static inline int SettingsChild(int *pt){
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
  *pt = port;
  return fd;
}

