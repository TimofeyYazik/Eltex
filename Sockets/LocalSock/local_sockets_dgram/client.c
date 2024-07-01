#include <unistd.h>
#include <stdint.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/un.h>
#include <string.h>

#define ADDR_NAME "/server"
#define ADDR_NAME_CLIENT "/client"
#define SIZE_BUFF 50
#define handle_error(msg) \
           do { perror(msg); exit(EXIT_FAILURE); } while (0)

int main(){
  int cfd = socket(AF_LOCAL, SOCK_DGRAM, 0);
  if(cfd < 0){
    handle_error("sockets");
  }
  if (access(ADDR_NAME_CLIENT, F_OK) == 0) {
    if (unlink(ADDR_NAME_CLIENT) == -1) {
      handle_error("unlink");      
    }
  }
  struct sockaddr_un serv_sock, client_sock;
  memset(&serv_sock, 0 , sizeof(serv_sock));
  memset(&client_sock, 0 , sizeof(client_sock));
  serv_sock.sun_family = AF_LOCAL;
  strncpy(serv_sock.sun_path, ADDR_NAME, sizeof(ADDR_NAME) - 1);
  client_sock.sun_family = AF_LOCAL;
  strncpy(client_sock.sun_path, ADDR_NAME_CLIENT, sizeof(ADDR_NAME_CLIENT) - 1);
  if(bind(cfd, (struct sockaddr *) &client_sock, sizeof(client_sock)) < 0){
    handle_error("bind");
  }
  if(connect(cfd, (struct sockaddr *) &serv_sock, sizeof(serv_sock)) < 0){
    handle_error("connect");    
  }
  char buff[SIZE_BUFF];
  strcpy(buff, "hello");
  send(cfd, buff, SIZE_BUFF, 0);
  recv(cfd, buff, SIZE_BUFF, 0);
  printf("%s\n", buff);
  send(cfd, buff, SIZE_BUFF, 0);
  close(cfd);
  unlink(client_sock.sun_path);
  exit(EXIT_SUCCESS);
}
