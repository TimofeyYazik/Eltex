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

#define PORT 6666
#define IP_ADDRES "127.0.0.1"
#define SIZE_BUFF 128
#define handler_error(text) \
do{ perror(text); exit(EXIT_FAILURE) } while(1);


void *ThreadStop(void *stop_p){
  int *stop = stop_p;
  while (*stop) {
    if(scanf("%d", stop) != 1){
      *stop = 0;
    }
  }
  return NULL;
}

int main(){
  int cfd = 0;
  int stop = 1;
  char buff[SIZE_BUFF] = {0};
  int ip_addres = 0;
  inet_pton(AF_INET, IP_ADDRES, &ip_addres);
  cfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
  struct sockaddr_in client_settings;
  client_settings.sin_family = AF_INET;
  client_settings.sin_addr.s_addr = ip_addres;
  client_settings.sin_port = htons(PORT);
  socklen_t size = sizeof(client_settings);
  if(cfd == -1){
  }
  pthread_t stop_client = 0;
  pthread_create(&stop_client, NULL, ThreadStop, &stop);
  printf("PRESS 0 (ZERO) CLIENT STOP\n");
  while(stop){
    recvfrom(cfd, buff, SIZE_BUFF, 0, (struct sockaddr*)&client_settings, &size);
    printf("%s\n", buff);
  }
  pthread_join(stop_client, NULL);
  close(cfd);
  exit(EXIT_SUCCESS);
}
