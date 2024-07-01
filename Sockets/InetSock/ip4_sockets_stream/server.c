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
  int sfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sfd < 0){
    handle_error("sockets");
  }
  struct sockaddr_in serv_sock;
  int ip_addres = 0;
  inet_pton(AF_INET, IP_ADDRES, &ip_addres);
  memset(&serv_sock, 0 , sizeof(serv_sock));  
  serv_sock.sin_family = AF_INET;
  serv_sock.sin_addr.s_addr = ip_addres;
  serv_sock.sin_port = htons(PORT);
  if(bind(sfd, (struct sockaddr*)&serv_sock, sizeof(serv_sock)) < 0){
      handle_error("bind");
  }
  socklen_t serv_len = sizeof(serv_sock);
  if(listen(sfd, 5) < 0){
    handle_error("listen");
  }
  int activ_fd = accept(sfd, (SA*) &serv_sock, &serv_len);
  char buff[SIZE_BUFF];
  recv(activ_fd, buff, SIZE_BUFF, 0);
  printf("%s\n", buff);
  strcpy(buff, "hi");
  send(activ_fd, buff, SIZE_BUFF, 0);
  recv(activ_fd, buff, SIZE_BUFF, 0);
  close(activ_fd);
  close(sfd);
  exit(EXIT_SUCCESS);
}
