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
  int stop = 1;
  time_t tm;
  pthread_t stop_thread;
  int flag_socket = 1;
  int cfd = socket(AF_INET, SOCK_DGRAM, 0);
  if(cfd == -1){
    handler_error("socket");
  }
  setsockopt(cfd, SOL_SOCKET, SO_BROADCAST, &flag_socket, sizeof(flag_socket));
  struct sockaddr_in client_endpoint;
  int ip_addres = 0;
  inet_pton(AF_INET, IP_ADDRES, &ip_addres);
  struct ip_mreq mreq;
  mreq.imr_multiaddr.s_addr = ip_addres;
  mreq.imr_interface.s_addr = INADDR_ANY;
  setsockopt(cfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));
  client_endpoint.sin_port = htons(PORT);
  client_endpoint.sin_family = AF_INET;
  client_endpoint.sin_addr.s_addr = ip_addres;
  char buff[SIZE_BUFF] = {0};
  if(bind(cfd, (SA*)&client_endpoint, sizeof(client_endpoint)) == -1){
    handler_error("bind");
  }
  recv(cfd, buff, SIZE_BUFF, 0);
  printf("%s", buff);
  close(cfd);
  exit(EXIT_SUCCESS);   
}


