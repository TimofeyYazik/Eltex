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

#define POOL_TREADS 20
#define SA struct sockaddr
#define PORT 6666
#define SIZE_BUFF 80
#define IP_ADDRES "127.0.0.2"
#define handler_error(text)                                                    \
  do {                                                                         \
    perror(text);                                                              \
    exit(EXIT_FAILURE);                                                        \
  } while (0);

typedef struct {
  int port;
  int busy;
} ServersInfo;

volatile int stop = 1;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
ServersInfo serv[POOL_TREADS] = {0};

static inline void FillSock(struct sockaddr_in *s){
  inet_pton(AF_INET, IP_ADDRES, &s->sin_addr.s_addr);
  s->sin_family = AF_INET;
  s->sin_port = htons(PORT);
}



void BindChild(int fd, struct sockaddr_in *server, int *port, int *serv_num){
  while (1) {
    if (bind(fd, (SA *)server, sizeof(*server)) == -1) {
      if (errno == EADDRINUSE) {
        (*port)++;
        server->sin_port = htons(*port);
        continue;
      } else {
        handler_error("bind");
      }
    }
    break;
  }
  for (int i = 0; i < POOL_TREADS; i++) {
    if (serv[i].port == 0) {
      serv[i].port = *port;
      *serv_num = i;
      break;
    }
  }
}

void ExchangeMessage(int fd, int serv_num){
  struct sockaddr_in client_endpoint;
  char buff[SIZE_BUFF] = {0};
  time_t time_now;
  
  socklen_t client_size = sizeof(client_endpoint);
  while (stop) {
    if (serv[serv_num].busy == 0) {
      usleep(100);
      continue;
    }
    printf("START SERVED CLIENT\n");
    while (1) {
      if(recvfrom(fd, buff, SIZE_BUFF, 0, (SA *)&client_endpoint, &client_size) == -1){
        perror("recvfrom thread");
        continue;
      }
      printf("Received from client %s\n", buff);
      if (!strcmp(buff, "exit")) {
        serv[serv_num].busy = 0;
        printf("Client is out\n");
        break;
      } else {
        time(&time_now);
        strncpy(buff, ctime(&time_now), 79);
        if(sendto(fd, buff, SIZE_BUFF, 0, (SA *)&client_endpoint, client_size) == -1){
          perror("sendto thread");
          break;
        }
        printf("Sent to client %s\n", buff);
      }
    }
  }
}


void *ChildServer(void *port_p) {
  int *port = port_p;
  int thread_sfd = socket(AF_INET, SOCK_DGRAM, 0);
  struct sockaddr_in server_settings, client_settings;
  server_settings.sin_family = AF_INET;
  inet_pton(AF_INET, IP_ADDRES, &server_settings.sin_addr);
  server_settings.sin_port = htons(*port);
  int port_thread = 0;
  socklen_t client_size = sizeof(client_settings);
  int serv_num = 0;
  pthread_mutex_lock(&mutex);
  BindChild(thread_sfd, &server_settings, port, &serv_num);
  pthread_mutex_unlock(&mutex);
  ExchangeMessage(thread_sfd, serv_num);
  close(thread_sfd);
  printf("THREAD IS OUT\n");
  return NULL;
}

void *StopServer(void *null) {
  while (stop) {
    if (scanf("%d", &stop) != 1) {
      stop = 0;
    }
  }
  int main_sfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (main_sfd == -1) {
    handler_error("socket");
  }
  struct sockaddr_in server_settings, client_settings;
  server_settings.sin_family = AF_INET;
  server_settings.sin_port = htons(PORT);
  inet_pton(AF_INET, IP_ADDRES, &server_settings.sin_addr);
  char buff[SIZE_BUFF] = {0};
  strcpy(buff, "close");
  sendto(main_sfd, buff, SIZE_BUFF, 0, (SA *)&server_settings, sizeof(server_settings));

  return NULL;
}


void ListenLoop(int main_sfd){
  char buff[SIZE_BUFF] = {0};
  socklen_t size_len_client = sizeof(struct sockaddr_in);
  struct sockaddr_in client_endpoint;
  int i = 0;
  while (stop) {
    if(recvfrom(main_sfd, buff, SIZE_BUFF, 0, (SA *)&client_endpoint, &size_len_client) == -1){
      perror("recvfrom");
      continue;
    }
    if (!strcmp(buff, "close"))
      break;
    if (strcmp(buff, "conn"))
      continue;
    for (i = 0; i < POOL_TREADS; i++) {
      if (serv[i].busy == 0 && serv[i].port != 0) {
        serv[i].busy = 1;
        break;
      }
    }
    sprintf(buff, "%d", serv[i].port);
    if(sendto(main_sfd, buff, SIZE_BUFF, 0, (SA *)&client_endpoint, size_len_client) == -1){
      perror("sendto");
      continue;
    }
  }
}

int main() {
  pthread_t arr_treads[POOL_TREADS] = {0};
  int main_sfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (main_sfd == -1) {
    handler_error("socket");
  }
  struct sockaddr_in server_settings;
  FillSock(&server_settings);
  if (bind(main_sfd, (SA *)&server_settings, sizeof(server_settings)) == -1) {
    handler_error("bind");
  }
  socklen_t size_len_serv = sizeof(struct sockaddr_in);
  pthread_t stop_tread;
  pthread_create(&stop_tread, NULL, StopServer, NULL);
  int ports[POOL_TREADS] = {0};
  for (int i = 0; i < POOL_TREADS; i++) {
    ports[i] = PORT + i + 1;
    pthread_create(&arr_treads[i], NULL, ChildServer, &ports[i]);
  }

  printf("SERVER START WORK\n");
  printf("PRESS 0 (ZERO) SERVER STOP\n");
  ListenLoop(main_sfd);
  pthread_join(stop_tread, NULL);
  for (int i = 0; i < POOL_TREADS; i++) {
    pthread_join(arr_treads[i], NULL);
  }
  pthread_mutex_destroy(&mutex);
  close(main_sfd);
  printf("SERVER END WORK\n");
  exit(EXIT_SUCCESS);
}
