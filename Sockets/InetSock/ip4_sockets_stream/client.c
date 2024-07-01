#include <unistd.h>
#include <stdint.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SA struct sockaddr
#define PORT 8084
#define IP_ADDRES "127.0.0.1"
#define SIZE_BUFF 128
#define handle_error(msg) \
           do { perror(msg); exit(EXIT_FAILURE); } while (0)


int main(){
  int cfd = socket(AF_INET, SOCK_STREAM, 0);
  if(cfd < 0){
    handle_error("sockets");
  }
  struct sockaddr_in serv_sock;
  int ip_addres = 0;
  inet_pton(AF_INET, IP_ADDRES, &ip_addres);
  memset(&serv_sock, 0 , sizeof(serv_sock));  
  serv_sock.sin_family = AF_INET;
  serv_sock.sin_addr.s_addr = ip_addres;
  serv_sock.sin_port = htons(PORT);
  if(connect(cfd, (SA*) &serv_sock, sizeof(serv_sock)) < 0){
    handle_error("connect");
  }
  char buff[SIZE_BUFF];
  strcpy(buff, "hello");
  send(cfd, buff, SIZE_BUFF, 0);
  recv(cfd, buff, SIZE_BUFF, 0);
  printf("%s\n", buff);
  send(cfd, buff, SIZE_BUFF, 0);
  close(cfd);
  exit(EXIT_SUCCESS);
}
