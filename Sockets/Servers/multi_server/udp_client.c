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



int main(){
  int ip_addres = 0;
  inet_pton(AF_INET, IP_ADDRES, &ip_addres);
  int cfd = socket(AF_INET, SOCK_DGRAM, 0);
  if(cfd == -1){
    handler_error("socket");
  }
  struct sockaddr_in server_connect;
  server_connect.sin_family = AF_INET;
  server_connect.sin_addr.s_addr = ip_addres;
  server_connect.sin_port = htons(PORT);
  char buff[SIZE_BUFF] = {0};
  strcpy(buff, "conn");
  socklen_t size_struct = sizeof(server_connect);
  if(connect(cfd, (SA*)&server_connect, sizeof(server_connect)) == -1){
    handler_error("ne vezet");
  }
  send(cfd, buff, SIZE_BUFF, 0);
  recv(cfd, buff, SIZE_BUFF, 0);
  printf("%s", buff);
  close(cfd);
  exit(EXIT_SUCCESS);
}
