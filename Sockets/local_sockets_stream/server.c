#include <stdint.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/un.h>
#include <string.h>

#define ADDR_NAME "/server"
#define LISTEN_BACKLOG 10
#define SIZE_BUFF 50
#define handle_error(msg) \
           do { perror(msg); exit(EXIT_FAILURE); } while (0)

int main(){
  int cfd = 0;
  int sfd = socket(AF_LOCAL, SOCK_STREAM, 0);
  if(sfd < 0){
    handle_error("sockets");
  }
  struct sockaddr_un serv_sock, client_sock;
  socklen_t len_sock = sizeof(serv_sock);
  memset(&serv_sock, 0 , len_sock);
  serv_sock.sun_family = AF_LOCAL;
  strncpy(serv_sock.sun_path, ADDR_NAME, sizeof(ADDR_NAME) - 1);
  if(bind(sfd, (struct sockaddr*)&serv_sock, len_sock) < 0){
      handle_error("sockets");
  }
  listen(sfd,LISTEN_BACKLOG);
  char buff[SIZE_BUFF];
  cfd = accept(sfd, (struct sockaddr*) &client_sock, sizeof(client_sock));
  recv(cfd, buff, SIZE_BUFF, 0);
  printf("%s\n", buff);
  strcpy(buff, "hi");
  send(cfd, buff, SIZE_BUFF, 0);
  recv(cfd, buff, SIZE_BUFF, 0);
  close(cfd);
  close(sfd);
  exit(EXIT_SUCCESS);
}
