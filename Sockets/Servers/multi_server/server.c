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

#define SA struct sockaddr
#define PORT_UDP 6666
#define MAX_EVENTS 10
#define PORT_TCP 6667
#define SIZE_BUFF 80
#define IP_ADDRES "127.0.0.1"
#define handler_error(text) \
  do { \
    perror(text); \
    exit(EXIT_FAILURE); \
  } while (0);

void *StopServer(void *null) {
  int stop = 1;
  while (stop) {
    if (scanf("%d", &stop) != 1) {
      stop = 0;
    }
  }
  int ip_addres = 0;
  inet_pton(AF_INET, IP_ADDRES, &ip_addres);
  int cfd = socket(AF_INET, SOCK_DGRAM, 0);
  if(cfd == -1){
    handler_error("socket");
  }
  struct sockaddr_in server_connect;
  server_connect.sin_family = AF_INET;
  server_connect.sin_addr.s_addr = ip_addres;
  server_connect.sin_port = htons(PORT_UDP);
  char buff[SIZE_BUFF] = {0};
  strcpy(buff, "close");
  socklen_t size_struct = sizeof(server_connect);
  if(connect(cfd, (SA*)&server_connect, sizeof(server_connect)) == -1){
    handler_error("ne vezet");
  }
  send(cfd, buff, SIZE_BUFF, 0);
  close(cfd);
  return NULL;
}


int main() {
  int ip_address = 0;
  inet_pton(AF_INET, IP_ADDRES, &ip_address);

  int sfd_tcp = socket(AF_INET, SOCK_STREAM, 0);
  int sfd_udp = socket(AF_INET, SOCK_DGRAM, 0);
  if (sfd_tcp == -1 || sfd_udp == -1) {
    handler_error("socket");
  }


  struct sockaddr_in server_udp, server_tcp;
  struct sockaddr_in client_udp;
  memset(&server_tcp, 0, sizeof(server_tcp));
  memset(&server_udp, 0, sizeof(server_udp));
  memset(&client_udp, 0, sizeof(client_udp));

  server_udp.sin_family = AF_INET;
  server_udp.sin_addr.s_addr = ip_address;
  server_udp.sin_port = htons(PORT_UDP);

  server_tcp.sin_family = AF_INET;
  server_tcp.sin_addr.s_addr = ip_address;
  server_tcp.sin_port = htons(PORT_TCP);

  if (bind(sfd_tcp, (SA *)&server_tcp, sizeof(server_tcp)) == -1) {
    handler_error("bind");
  }
  if (bind(sfd_udp, (SA *)&server_udp, sizeof(server_udp)) == -1) {
    handler_error("bind");
  }
  if (listen(sfd_tcp, 5) == -1) {
    handler_error("listen");
  }

  socklen_t len = sizeof(server_udp);
  pthread_t stop_thread;
  pthread_create(&stop_thread, NULL, StopServer, NULL);

  struct epoll_event ev, ev1;
  ev.events = EPOLLIN;
  ev.data.fd = sfd_udp;
  ev1.events = EPOLLIN;
  ev1.data.fd = sfd_tcp;
  struct epoll_event events[MAX_EVENTS];

  int epfd = epoll_create1(0);
  if (epfd == -1) {
      handler_error("epoll_create1");
  }
  if (epoll_ctl(epfd, EPOLL_CTL_ADD, sfd_udp, &ev) == -1) {
      handler_error("epoll_ctl: sfd_udp");
  }
  if (epoll_ctl(epfd, EPOLL_CTL_ADD, sfd_tcp, &ev1) == -1) {
      handler_error("epoll_ctl: sfd_tcp");
  }

  printf("SERVER START WORK\n");
  printf("PRESS 0 (ZERO) SERVER STOP\n");

  time_t tm = 0;
  int recv_r = 0;
  int conn_sock = 0;
  socklen_t size = sizeof(client_udp);
  char buff[SIZE_BUFF] = {0};
  int stop = 0;
  while (!stop) {
    int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
    if (nfds == -1) {
        perror("epoll_wait");
        close(epfd);
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < nfds; i++) {
      if (events[i].data.fd == sfd_tcp) {
        conn_sock = accept(sfd_tcp, (SA*)&client_udp, &size);
        if (conn_sock == -1) {
          perror("accept");
          continue;
        }
        time(&tm);
        strncpy(buff, ctime(&tm), SIZE_BUFF - 1);
        send(conn_sock, buff, SIZE_BUFF, 0);
        close(conn_sock);
      } else if (events[i].data.fd == sfd_udp) {
        socklen_t size_client = sizeof(client_udp);
        recv_r = recvfrom(sfd_udp, buff, SIZE_BUFF, 0, (SA*)&client_udp, &size_client);
        if(!strcmp(buff, "close")){
            stop = 1;
            break;
        }
        if (recv_r == -1) {
            perror("recvfrom");
            break;
        }
        time(&tm);
        strncpy(buff, ctime(&tm), SIZE_BUFF - 1);
        sendto(sfd_udp, buff, SIZE_BUFF, 0, (SA*)&client_udp, size_client);
      }
    }
  }

  pthread_join(stop_thread, NULL);
  close(sfd_tcp);
  close(sfd_udp);
  close(epfd);
  printf("SERVER END WORK\n");
  exit(EXIT_SUCCESS);
}
