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
#define IP_ADDRES "127.0.0.1"
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
  struct sockaddr_in server_enpoint;
  FillSock(&server_enpoint);
  char buff[SIZE_BUFF] = {0};
  if(connect(cfd, (SA*)&server_enpoint, sizeof(server_enpoint)) == -1){
    handler_error("ne vezet");
  }
  send(cfd, buff, SIZE_BUFF, 0);
  recv(cfd, buff, SIZE_BUFF, 0);
  printf("%s", buff);
  close(cfd);
  exit(EXIT_SUCCESS);
}

