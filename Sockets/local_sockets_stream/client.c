#include <unistd.h>
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
  int cfd = socket(AF_LOCAL, SOCK_STREAM, 0);
  if(cfd < 0){
    handle_error("sockets");
  }
  struct sockaddr_un serv_sock;
  socklen_t len_sock = sizeof(serv_sock);
  memset(&serv_sock, 0 , len_sock);
  serv_sock.sun_family = AF_LOCAL;
  strncpy(serv_sock.sun_path, ADDR_NAME, sizeof(ADDR_NAME) - 1);
  if(connect(cfd, (struct sockaddr*) &serv_sock, sizeof(serv_sock)) < 0){
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
