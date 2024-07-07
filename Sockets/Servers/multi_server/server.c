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
#include <sys/epoll.h>

#include "double_list/list.h"

#define POOL_TREADS 50
#define SA struct sockaddr
#define PORT_UDP 6666
#define PORT_TCP 6667
#define SIZE_BUFF 80
#define IP_ADDRES "127.0.0.1"
#define handler_error(text)                                                    \
  do {                                                                         \
    perror(text);                                                              \
    exit(EXIT_FAILURE);                                                        \
  } while (0);

volatile int stop = 1;
void *StopServer(void *s) {
  int *ip = s;
  int ip_addres = *ip;
  while (stop) {
    if (scanf("%d", &stop) != 1) {
      stop = 0;
    }
  }
}

int main() {
  int ip_addres = 0;
  inet_pton(AF_INET, IP_ADDRES, &ip_addres);
  int sfd_tsp = socket(AF_INET, SOCK_STREAM, 0);
  int sfd_udp = socket(AF_INET, SOCK_DGRAM, 0);
  if (sfd_tsp == -1 || sfd_udp == -1) {
    handler_error("socket");
  }

  struct sockaddr_in server_udp, server_tcp;
  memset(&server_tcp, 0, sizeof(server_tcp));
  memset(&server_udp, 0, sizeof(server_udp));
  server_udp.sin_family = AF_INET;
  server_udp.sin_addr.s_addr = ip_addres;
  server_udp.sin_port = htons(PORT_UDP);
  server_tcp.sin_family = AF_INET;
  server_tcp.sin_addr.s_addr = ip_addres;
  server_tcp.sin_port = htons(PORT_TCP);
  
  if (bind(sfd_tsp, (SA *)&server_tcp, sizeof(server_tcp)) == -1) {
    handler_error("bind");
  }
  if (bind(sfd_udp, (SA *)&server_udp, sizeof(server_udp)) == -1) {
    handler_error("bind");
  }
  if (listen(sfd_tsp, 5) == -1) {
    handler_error("listen");
  }

  socklen_t len = sizeof(server_udp);
  pthread_t stop_tread;
  pthread_create(&stop_tread, NULL, StopServer, (void *)&ip_addres);


  

  printf("SERVER START WORK\n");
  printf("PRESS 0 (ZERO) SERVER STOP\n");

  int recv_r = 0;
  char buff[SIZE_BUFF] = {0};
  while (stop) {
  }
  pthread_join(stop_tread, NULL);
  for (int i = 0; i < POOL_TREADS; i++) {
  }
  close(sfd_tsp);
  close(sfd_udp);
  printf("SERVER END WORK\n");
  exit(EXIT_SUCCESS);
}
