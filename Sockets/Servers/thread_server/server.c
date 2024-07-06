#include <signal.h>
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
#define PORT 6666
#define LISTEN_USERS 8
#define SIZE_BUFF 80
#define IP_ADDRES "127.0.0.1"
#define handler_error(text) \
          do { perror(text); exit(EXIT_FAILURE); } while(0);

volatile int stop = 1;
typedef struct{
  int *arr;
  int len;
  int size;
} ActiveFD;

typedef struct{
  pthread_t *arr;
  int len;
  int size;
} Thread;

void *ChildServer(void *fd){
  int *active_fd = (int *)fd;
  time_t time_now = 0;
  char buff[SIZE_BUFF] = {0};
  while (1) {
    recv(*active_fd, buff, SIZE_BUFF, 0);
    printf("RECERV CLIENT: %d\n", *active_fd);
    if(!strcmp(buff, "exit")){
      send(*active_fd, buff, SIZE_BUFF, 0);
      break;
    } else {
      time(&time_now);
      strcpy(buff, ctime(&time_now));
      send(*active_fd, buff, SIZE_BUFF, 0);
      printf("SEND CLIENT: %d\n", *active_fd);
    }  
  }
  printf("CLIENT IS OUT: %d\n", *active_fd);
  close(*active_fd);  
  return NULL;
  
}


void *StopServer(void *ip){
  int *ip_addres = ip;
  while(1){
    if(scanf("%d", &stop) != 1){
      stop = 0;
    }
    if(stop == 0) break;
  }  
  int cfd = socket(AF_INET, SOCK_STREAM, 0);
  if(cfd == -1){
    handler_error("socket");
  }
  struct sockaddr_in server_connect;
  server_connect.sin_family = AF_INET;
  server_connect.sin_addr.s_addr = *ip_addres;
  server_connect.sin_port = htons(PORT);
  char buff[SIZE_BUFF] = {0};
  strcpy(buff, "close");
  if(connect(cfd, (SA*)&server_connect, sizeof(server_connect)) == -1){
    handler_error("ne vezet");
  }
  send(cfd, buff, SIZE_BUFF, 0);
  close(cfd);  
  return NULL;
}

void AddFD(int fd, ActiveFD *obj){
  if(obj->len == obj->size){
    obj->size = obj->size * 3 / 2;
    obj->arr = realloc(obj->arr, obj->size);
  }
  obj->arr[obj->len] = fd;
  obj->len++;
}

void AddThread(int *fd, Thread *obj){
  if(obj->len == obj->size){
    obj->size = obj->size * 3 / 2;
    obj->arr = realloc(obj->arr, obj->size);
  }
  pthread_create(&obj->arr[obj->len], NULL, ChildServer, (void *)fd);
  obj->len++;
}

int main(){
  ActiveFD obj_act;
  obj_act.size = 100;
  obj_act.len = 0;
  obj_act.arr = calloc(obj_act.size, sizeof(int));
  Thread obj_thread;
  obj_thread.len = 0;
  obj_thread.size = 100;
  obj_thread.arr = calloc(obj_thread.size, sizeof(pthread_t ));
  int main_sfd = socket(AF_INET, SOCK_STREAM, 0);
  if(main_sfd == -1){
    handler_error("socket");
  }
  struct sockaddr_in server_settings;
  server_settings.sin_family = AF_INET;
  server_settings.sin_port = htons(PORT);
  inet_pton(AF_INET, IP_ADDRES, &server_settings.sin_addr);

  if(bind(main_sfd,(SA *)&server_settings, sizeof(server_settings)) == -1){
    handler_error("bind");
  }
  if(listen(main_sfd, LISTEN_USERS) == -1){
    handler_error("listen");
  }
  socklen_t len = sizeof(server_settings);
  int counter = 0;
  pthread_t stop_tread;
  pthread_create(&stop_tread, NULL, StopServer, (void *) &server_settings.sin_addr);
  printf("SERVER START WORK\n");
  printf("PRESS 0 (ZERO) SERVER STOP\n");
  char buff[SIZE_BUFF] = {0};
  while(stop){
    int active_fd = accept(main_sfd, (SA *)&server_settings, &len);
    recv(active_fd, buff, SIZE_BUFF, 0);
    if(!strcmp(buff, "close")) break;
    if(strcmp(buff, "conn")) continue;
    printf("NEW CLIENT: %d\n", active_fd);
    AddFD(active_fd, &obj_act);
    AddThread(&obj_act.arr[obj_act.len], &obj_thread);
  }
  pthread_join(stop_tread, NULL);
  for(int i = 0; i < obj_thread.len; i++){
    pthread_join(obj_thread.arr[i], NULL);
  }
  close(main_sfd);
  free(obj_act.arr);
  free(obj_thread.arr);
  printf("SERVER END WORK\n");
  exit(EXIT_SUCCESS);
}
