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

#define SA struct sockaddr
#define PORT 6666
#define SIZE_BUFF 80
#define IP_ADDRES "255.255.255.255"
#define handler_error(text)                                                    \
  do { perror(text); exit(EXIT_FAILURE); } while (0);

void *StopServer(void *p_stop){
  int *stop = p_stop;
  while(*stop){
    if(scanf("%d", stop) != 1){
      *stop = 0;
    }
  }
  return NULL;
}

int main(){
  int stop = 1;
  time_t tm;
  pthread_t stop_thread;
  int flag_socket = 1;
  int sfd = socket(AF_INET, SOCK_DGRAM, 0);
  if(sfd == -1){
    handler_error("socket");
  }
  setsockopt(sfd, SOL_SOCKET, SO_BROADCAST, &flag_socket, sizeof(flag_socket));
  struct sockaddr_in client_endpoint;
  int ip_addres = 0;
  inet_pton(AF_INET, IP_ADDRES, &ip_addres);
  client_endpoint.sin_port = htons(PORT);
  client_endpoint.sin_family = AF_INET;
  char buff[SIZE_BUFF] = {0};
  client_endpoint.sin_addr.s_addr = ip_addres;
  pthread_create(&stop_thread, NULL, StopServer, &stop);

  printf("SERVER START WORK\n");
  printf("PRESS 0 (ZERO) SERVER STOP\n");
  
  while(stop){
    time(&tm);
    strcpy(buff, ctime(&tm));
    sendto(sfd, buff, SIZE_BUFF, 0, (SA*)&client_endpoint, sizeof(client_endpoint));
    sleep(5);
  }
  pthread_join(stop_thread, NULL);
  close(sfd);
  exit(EXIT_SUCCESS);   
}


