#include <unistd.h>
#include <stdint.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/un.h>
#include <string.h>

#define ADDR_NAME "/server"
#define SIZE_BUFF 50
#define handle_error(msg) \
           do { perror(msg); exit(EXIT_FAILURE); } while (0)

int main(){
  int sfd = socket(AF_LOCAL, SOCK_DGRAM, 0);
  if(sfd < 0){
    handle_error("sockets");
  }
  if (access(ADDR_NAME, F_OK) == 0) {
    if (unlink(ADDR_NAME) == -1) {
      handle_error("unlink");      
    }
  }
  struct sockaddr_un serv_sock, client_sock;
  memset(&serv_sock, 0 , sizeof(serv_sock));  
  memset(&client_sock, 0 , sizeof(client_sock));
  serv_sock.sun_family = AF_LOCAL;
  strncpy(serv_sock.sun_path, ADDR_NAME, sizeof(ADDR_NAME) - 1);
  if(bind(sfd, (struct sockaddr*)&serv_sock, sizeof(serv_sock)) < 0){
      handle_error("sockets");
  }
  char buff[SIZE_BUFF];
  socklen_t sk_len = sizeof(client_sock);
  recvfrom(sfd, buff, SIZE_BUFF, 0, (struct sockaddr *) &client_sock, &sk_len);
  printf("%s\n", buff);
  strcpy(buff, "hi");
  sendto(sfd, buff, SIZE_BUFF, 0, (struct sockaddr *) &client_sock, sk_len);
  recvfrom(sfd, buff, SIZE_BUFF, 0, (struct sockaddr *) &client_sock, &sk_len);
  close(sfd);
  unlink(serv_sock.sun_path);
  unlink(client_sock.sun_path);
  exit(EXIT_SUCCESS);
}
