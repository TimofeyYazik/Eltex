#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/signal.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#define SA struct sockaddr
#define PORT 6666
#define SIZE_BUFF 80
#define IP_ADDRES "224.0.0.1"
#define handler_error(text)                                                    \
  do { perror(text); exit(EXIT_FAILURE); } while (0);

int main(){
  time_t tm = 0;
  pthread_t stop_thread = 0;
  int flag_socket = 1;
  int cfd = socket(AF_INET, SOCK_DGRAM, 0);
  if(cfd == -1){
    handler_error("socket");
  }
  setsockopt(cfd, SOL_SOCKET, SO_BROADCAST, &flag_socket, sizeof(flag_socket));
  struct sockaddr_in server_endpoint;
  int ip_addres = 0;
  inet_pton(AF_INET, IP_ADDRES, &ip_addres);
  struct ip_mreq mreq;
  mreq.imr_multiaddr.s_addr = ip_addres;
  mreq.imr_interface.s_addr = INADDR_ANY;
  setsockopt(cfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));
  server_endpoint.sin_port = htons(PORT);
  server_endpoint.sin_family = AF_INET;
  server_endpoint.sin_addr.s_addr = ip_addres;
  char buff[SIZE_BUFF] = {0};
  if(bind(cfd, (SA*)&server_endpoint, sizeof(server_endpoint)) == -1){
    handler_error("bind");
  }
  recv(cfd, buff, SIZE_BUFF, 0);
  printf("%s", buff);
  close(cfd);
  exit(EXIT_SUCCESS);   
}
