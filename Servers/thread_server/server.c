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

#include "double_list/list.h"

#define SA struct sockaddr
#define PORT 6666
#define SIZE_BUFF 8
#define IP_ADDRES "127.0.0.1"
#define handler_error(text) \
          do { perror(text); exit(EXIT_FAILURE); } while(0);


void *ChildServer(void *fd){
  printf("CHILD SERVER\n");
  int *active_fd = (int *)fd;
  char time_buff[80] = {0};
  time_t time_now;
  char buff[SIZE_BUFF];
  while (1) {
    printf("START RECEIVE\n");
    recv(*active_fd, buff, SIZE_BUFF, 0);
    printf("END RECEIVE\n");
    if(!strcmp(buff, "exit")){
      break;
    } else {
      time(&time_now);
      strcpy(time_buff, ctime(&time_now));
      printf("SEND\n");
      send(*active_fd, (void *)time_buff, 80, 0);
    }  
  }
  printf("END POTOK");
  close(*active_fd);  
  return NULL;
  
}

int stop = 1;

void HandlerUserSignal(int a){
  stop = 0;
}


int main(){
  signal(SIGUSR1, HandlerUserSignal);
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
  if(listen(main_sfd, 10) == -1){
    handler_error("listen");
  }
  socklen_t len = sizeof(server_settings);
  int counter = 0;
  while(stop){
    printf("LOOP\n");
    int active_fd = accept(main_sfd, (SA *)&server_settings, &len);
    pthread_create(&arr_treads[counter], NULL, ChildServer, (void *)&active_fd); 
    counter++;
    if(counter == len_treads_arr){
      len_treads_arr *= 2;
      arr_treads = realloc(arr_treads, len_treads_arr);
    }
  }
  printf("END PROGRAM\n");
  for(int i = 0; i < counter; i++){
    pthread_join(arr_treads[i], NULL);
  }
  close(main_sfd);
  exit(EXIT_SUCCESS);
}
