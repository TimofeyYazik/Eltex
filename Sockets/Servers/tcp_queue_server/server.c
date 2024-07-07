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

#define POOL_TREADS 20
#define SA struct sockaddr
#define PORT 6666
#define SIZE_BUFF 80
#define IP_ADDRES "127.0.0.1"
#define handler_error(text)                                                    \
  do {                                                                         \
    perror(text);                                                              \
    exit(EXIT_FAILURE);                                                        \
  } while (0);



ListServer *head = NULL;
volatile int stop = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *StopServer(void *null){
  scanf("%d", &stop);
  stop = 1;
  struct sockaddr_in serv;
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  serv.sin_family = AF_INET;
  serv.sin_port = htons(PORT);
  inet_pton(AF_INET, IP_ADDRES, &serv.sin_addr);
  connect(sock, (SA*)&serv, sizeof(serv));
  char buff[SIZE_BUFF] = {0};
  strcpy(buff, "close");
  send(sock, buff, SIZE_BUFF, 0);
  close(sock);
  return NULL;
}

void *ChildServer(void *null){
  int fd = 0;
  time_t tm = 0;
  char buff[SIZE_BUFF] = {0};
  ListServer *f = NULL;
  while (!stop) {
    pthread_mutex_lock(&mutex);
    f = SearchFree(head);
    if(f == NULL){
      pthread_mutex_unlock(&mutex);
      usleep(100);
      continue;
    }
    fd = f->active_fd;
    Remove(f);
    pthread_mutex_unlock(&mutex);
    time(&tm);
    strncpy(buff, ctime(&tm), SIZE_BUFF - 1);
    send(fd, buff, SIZE_BUFF, 0);
    close(fd);
  }
  return NULL;
}

int main(){
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  pthread_t servers[POOL_TREADS] = {0};
  char buff[SIZE_BUFF] = {0};
  struct sockaddr_in serv;
  serv.sin_family = AF_INET;
  serv.sin_port = htons(PORT);
  inet_pton(AF_INET, IP_ADDRES, &serv.sin_addr);
  head = CreateList();
  if(bind(sock, (SA*)&serv, sizeof(serv)) == -1){
    close(sock);
    handler_error("bind");
  }
  listen(sock, 5);
  socklen_t size = sizeof(serv);
  pthread_t st = 0;
  pthread_create(&st, NULL, StopServer, NULL);
  for(int i = 0; i < POOL_TREADS; i++){
    pthread_create(&servers[i], NULL, ChildServer, NULL);
  }
  printf("TYPE KEY TO STOP SERVER\n");
  while(!stop){
    int asock = accept(sock, (SA*)&serv, &size);
    ListServer *nel = malloc(sizeof(ListServer));
    nel->active_fd = asock;
    recv(asock, buff, SIZE_BUFF, 0);
    if(!strcmp(buff, "close")) break;;
    pthread_mutex_lock(&mutex);
    InsertEnd(head, nel);
    pthread_mutex_unlock(&mutex);
  }
  pthread_mutex_destroy(&mutex);
  close(sock);
  exit(EXIT_SUCCESS);
}
