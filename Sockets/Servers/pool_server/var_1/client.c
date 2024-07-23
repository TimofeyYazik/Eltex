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
#define SIZE_BUFF 80
#define IP_ADDRES "127.0.0.2"
#define handler_error(text) \
          do { perror(text); exit(EXIT_FAILURE); } while(0);


static inline void FillSock(struct sockaddr_in *s){
  inet_pton(AF_INET, IP_ADDRES, &s->sin_addr.s_addr);
  s->sin_family = AF_INET;
  s->sin_port = htons(PORT);
}

int main(){
  int cfd = socket(AF_INET, SOCK_DGRAM, 0);
  if(cfd == -1){
    handler_error("socket");
  }
  struct sockaddr_in server_endpoint;
  FillSock(&server_endpoint);
  char buff[SIZE_BUFF] = {0};
  strcpy(buff, "conn");
  socklen_t size_struct = sizeof(server_endpoint);
  printf("START CLIENT\n");
  sendto(cfd, buff, SIZE_BUFF, 0, (SA*)&server_endpoint, sizeof(SA));
  recvfrom(cfd, buff, SIZE_BUFF, 0, (SA*)&server_endpoint, &size_struct);
  int port = 0;
  sscanf(buff, "%d", &port);
  server_endpoint.sin_port = htons(port);
  if(connect(cfd, (SA*)&server_endpoint, sizeof(server_endpoint)) == -1){
    handler_error("ne vezet");
  }
  printf("type 'time' to display the time\n");
  printf("type 'exit' to exit\n");
  while(1){
    scanf("%7s", buff);
    if(!strcmp(buff, "exit")){
      send(cfd, buff, SIZE_BUFF, 0);
      break;
    }    
    if(!strcmp(buff, "time")){
      send(cfd, buff, SIZE_BUFF, 0);
      recv(cfd, buff, SIZE_BUFF, 0);
      printf("%s", buff);
    }
  }
  close(cfd);
  exit(EXIT_SUCCESS);
}
