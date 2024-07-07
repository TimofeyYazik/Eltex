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
#define SIZE_BUFF 80
#define IP_ADDRES "127.0.0.1"
#define handler_error(text)                                                    \
  do {                                                                         \
    perror(text);                                                              \
    exit(EXIT_FAILURE);                                                        \
  } while (0);

void *StopServer(void *null);
void *ChildServer(void *null);

ListServer *head = NULL;
volatile int stop = 1;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;



void *ChildServer(void *null){
  int fd = 0;
  time_t tm = 0;
  char buff[SIZE_BUFF] = {0};
  ListServer *f = NULL;
  while (1) {
    pthread_mutex_lock(&mutex);
    SearchFree(f);
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
  for(int i = 0; i < POOL_TREADS; i++){
    pthread_create(&servers[i], NULL, ChildServer, NULL);
  }
  while(1){
    int asock = accept(sock, (SA*)&serv, &size);
    ListServer *nel = malloc(sizeof(ListServer));
    nel->active_fd = asock;
    pthread_mutex_lock(&mutex);
    InsertEnd(head, nel);
    pthread_mutex_unlock(&mutex);
  }
  pthread_mutex_destroy(&mutex);
  close(sock);
  exit(EXIT_SUCCESS);
}
