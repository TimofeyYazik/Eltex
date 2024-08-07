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
#define SIZE_BUFF 108
#define handler_error(text) \
do{ perror(text); exit(EXIT_FAILURE); } while(1);


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
  cfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
  if(cfd == -1){
    handler_error("socket"); 
  }
  struct sockaddr_in client_settings;
  memset(&client_settings, 0, sizeof(client_settings));
  socklen_t size = sizeof(client_settings);
  pthread_t stop_client = 0;
  pthread_create(&stop_client, NULL, ThreadStop, &stop);
  printf("PRESS 0 (ZERO) CLIENT STOP\n");
  while(stop){
    recvfrom(cfd, buff, SIZE_BUFF, 0, (struct sockaddr*)&client_settings, &size);
    printf("%s\n", buff + 28);
  }
  pthread_join(stop_client, NULL);
  close(cfd);
  exit(EXIT_SUCCESS);
}
