#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>
#include <stdint.h>

#define PORT 6666
#define SOURCE_PORT 7777
#define IP_ADDRES "127.0.0.1"
#define SIZE_BUFF 128
#define SA struct sockaddr
#define handler_error(text) \
do{ perror(text); exit(EXIT_FAILURE); } while(1);


int main(){
  int cfd = 0;
  int stop = 1;
  char buff[SIZE_BUFF] = {0};
  int16_t ip_addres = 0;
  inet_pton(AF_INET, IP_ADDRES, &ip_addres);
  cfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
  if(cfd == -1){
    handler_error("socket");
  }
  struct sockaddr_in server_endpoint;
  server_endpoint.sin_family = AF_INET;
  server_endpoint.sin_addr.s_addr = ip_addres;
  server_endpoint.sin_port = htons(PORT);
  socklen_t size = sizeof(server_endpoint);
  pthread_t stop_client = 0;
  printf("PRESS 0 (ZERO) CLIENT STOP\n");

  int16_t *point_head = &buff[0];
  *point_head = htons(SOURCE_PORT);
  point_head++;
  *point_head = htons(PORT);
  point_head++;
  *point_head = SIZE_BUFF;
  point_head++;
  *point_head = 0;
  point_head -= 2;
  while(stop){
    scanf("%119s", buff + 8);
    if(!strcmp(buff + 8, "exit")) break;
    sendto(cfd, buff, SIZE_BUFF, 0, (SA*)&server_endpoint, size);
    recvfrom(cfd, buff, SIZE_BUFF, 0, (SA*)&server_endpoint, &size);
    if(*point_head == htons(SOURCE_PORT))
      printf("%s\n", buff);
  }
  pthread_join(stop_client, NULL);
  close(cfd);
  exit(EXIT_SUCCESS);
}
