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



int main(){
  int len_treads_arr = 100;
  pthread_t *arr_treads = malloc(len_treads_arr * sizeof(pthread_t));
  int ip_addres = 0;
  inet_pton(AF_INET, IP_ADDRES, &ip_addres);
  int main_sfd = socket(AF_INET, SOCK_STREAM, 0);
  if(main_sfd == -1){
    handler_error("socket");
  }
  struct sockaddr_in server_settings;
  server_settings.sin_family = AF_INET;
  server_settings.sin_addr.s_addr = ip_addres;
  server_settings.sin_port = htons(PORT);

  if(bind(main_sfd,(SA *)&server_settings, sizeof(server_settings)) == -1){
    handler_error("bind");
  }
  if(listen(main_sfd, LISTEN_USERS) == -1){
    handler_error("listen");
  }
  socklen_t len = sizeof(server_settings);
  int counter = 0;
  pthread_t stop_tread;
  pthread_create(&stop_tread, NULL, StopServer, (void *) &ip_addres);
  printf("SERVER START WORK\n");
  printf("PRESS 0 (ZERO) SERVER STOP\n");
  int len_fd_arr = 100;
  int *arr_active_fd = malloc(len_fd_arr * sizeof(int));
  int thread_fd = 0;
  char buff[SIZE_BUFF] = {0};
  while(stop){
    int active_fd = accept(main_sfd, (SA *)&server_settings, &len);
    recv(active_fd, buff, SIZE_BUFF, 0);
    if(!strcmp(buff, "close")) break;
    if(strcmp(buff, "conn")) continue;
    arr_active_fd[thread_fd] = active_fd;
    printf("NEW CLIENT: %d\n", active_fd);
    pthread_create(&arr_treads[counter], NULL, ChildServer, (void *)&arr_active_fd[thread_fd]);
    thread_fd++;
    if(thread_fd == len_fd_arr){
      len_fd_arr *= 2;
      arr_active_fd = realloc(arr_active_fd, len_fd_arr);
    } 
    counter++;
    if(counter == len_treads_arr){
      len_treads_arr *= 2;
      arr_treads = realloc(arr_treads, len_treads_arr);
    }
  }
  pthread_join(stop_tread, NULL);
  for(int i = 0; i < counter; i++){
    pthread_join(arr_treads[i], NULL);
  }
  close(main_sfd);
  printf("SERVER END WORK\n");
  exit(EXIT_SUCCESS);
}
