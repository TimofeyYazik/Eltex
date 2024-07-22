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
#define PORT 6667
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
  int cfd = socket(AF_INET, SOCK_STREAM, 0);
  if(cfd == -1){
    handler_error("socket");
  }
  struct sockaddr_in server_endpoint;
  FillSock(&server_endpoint);
  char buff[SIZE_BUFF] = {0};
  if(connect(cfd, (SA*)&server_endpoint, sizeof(server_endpoint)) == -1){
    handler_error("connect");
  }
  strcpy(buff, "conn");
  if(send(cfd, buff, SIZE_BUFF, 0) == -1){
    handler_error("send");
  }
  if(recv(cfd, buff, SIZE_BUFF, 0) == -1){
    handler_error("recv");
  }
  printf("%s", buff);
  close(cfd);
  exit(EXIT_SUCCESS);
}
