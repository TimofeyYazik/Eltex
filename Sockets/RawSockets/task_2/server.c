#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#define PORT 6666
#define IP_ADDRES "127.0.0.1"
#define SA struct sockaddr
#define SIZE_BUFF 10
#define handler_error(text) \
do{ perror(text); exit(EXIT_FAILURE); } while(1);


void *ThreadStop(void *stop_p){
  int *stop = stop_p;
  while (*stop) {
    if(scanf("%d", stop) != 1){
      *stop = 0;
    }
  }
  return NULL;
}

int main(){
  int sfd = 0;
  int stop = 1;
  char buff[SIZE_BUFF] = {0};
  int ip_addres = 0;
  inet_pton(AF_INET, IP_ADDRES, &ip_addres);
  sfd = socket(AF_INET, SOCK_DGRAM, 0);
  if(sfd == -1){
    handler_error("socket"); 
  }
  struct sockaddr_in server_settings, client_endpoint;
  server_settings.sin_family = AF_INET;
  server_settings.sin_addr.s_addr = ip_addres;
  server_settings.sin_port = htons(PORT);
  socklen_t size = sizeof(client_endpoint);
  pthread_t stop_client = 0;
  if(bind(sfd, (SA*) &server_settings, sizeof(server_settings)) == -1){
    handler_error("bind");
  }
  
  pthread_create(&stop_client, NULL, ThreadStop, &stop);
  
  printf("PRESS 0 (ZERO) CLIENT STOP\n");
  while(stop){
    recvfrom(sfd, buff, SIZE_BUFF, 0, (SA*)&client_endpoint, &size);
    printf("RECV\n");
    buff[0] = 'B';
    sendto(sfd, buff, SIZE_BUFF, 0, (SA*) &client_endpoint, sizeof(client_endpoint));
    printf("SEND\n");
  }
  pthread_join(stop_client, NULL);
  close(sfd);
  exit(EXIT_SUCCESS);
}

