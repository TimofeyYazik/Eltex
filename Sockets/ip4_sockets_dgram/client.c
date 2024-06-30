#include <unistd.h>
#include <stdint.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define IP_ADDRES "127.0.0.1"
#define SA struct sockaddr
#define PORT 8084
#define SIZE_BUFF 128
#define handle_error(msg) \
           do { perror(msg); exit(EXIT_FAILURE); } while (0)

int main(){
  int sfd = socket(AF_INET, SOCK_DGRAM, 0);
  if(sfd < 0){
    handle_error("sockets");
  }
  int ip_addres = 0;
  struct sockaddr_in serv_sock;
  memset(&serv_sock, 0 , sizeof(serv_sock));  
  inet_pton(AF_INET, IP_ADDRES, &ip_addres);
  serv_sock.sin_family = AF_INET;
  serv_sock.sin_addr.s_addr = ip_addres;
  serv_sock.sin_port = htons(PORT);
  if(connect(sfd, (SA*) &serv_sock, sizeof(serv_sock)) == 1){
    handle_error("connect");
  }
  char buff[SIZE_BUFF];
  strcpy(buff, "hello");
  socklen_t sk_len = sizeof(serv_sock);
  send(sfd, buff, SIZE_BUFF, 0);
  recv(sfd, buff, SIZE_BUFF, 0);
  printf("%s\n", buff);
  send(sfd, buff, SIZE_BUFF, 0);
  close(sfd);
  exit(EXIT_SUCCESS);
}
