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

void *ChildServer(void *port_p) {
  int *port = port_p;
  time_t time_now;
  char buff[SIZE_BUFF] = {0};

  int ip_addres = 0;
  inet_pton(AF_INET, IP_ADDRES, &ip_addres);
  int thread_sfd = socket(AF_INET, SOCK_DGRAM, 0);

  struct sockaddr_in server_settings, client_settings;
  server_settings.sin_family = AF_INET;
  server_settings.sin_addr.s_addr = ip_addres;
  server_settings.sin_port = htons(*port);
  int port_thread = 0;
  socklen_t client_size = sizeof(client_settings);
  pthread_mutex_lock(&mutex);
  while (1) {
    if (bind(thread_sfd, (SA *)&server_settings, sizeof(server_settings)) ==
        -1) {
      if (errno == EADDRINUSE) {
        (*port)++;
        server_settings.sin_port = htons(*port);
        continue;
      } else {
        handler_error("bind");
      }
    }
    break;
  }
  int serv_num = 0;
  for (int i = 0; i < POOL_TREADS; i++) {
    if (serv[i].port == 0) {
      serv[i].port = *port;
      serv_num = i;
      break;
    }
  }
  pthread_mutex_unlock(&mutex);

  while (stop) {
    if (serv[serv_num].busy == 0) {
      sleep(2);
      continue;
    }
    printf("START SERVED CLIENT\n");
    while (1) {
      if(recvfrom(thread_sfd, buff, SIZE_BUFF, 0, (SA *)&client_settings, &client_size) == -1){
        perror("recvfrom");
        continue;
      }
      printf("RECV CLIENT\n");
      if (!strcmp(buff, "exit")) {
        serv[serv_num].busy = 0;
        sendto(thread_sfd, buff, SIZE_BUFF, 0, (SA *)&client_settings, client_size);
        printf("STOP SERVED CLIENT\n");
        break;
      } else {
        time(&time_now);
        strncpy(buff, ctime(&time_now), 79);
        sendto(thread_sfd, buff, SIZE_BUFF, 0, (SA *)&client_settings, client_size);
        printf("SEND CLIENT\n");
      }
    }
  }
  close(thread_sfd);
  printf("THREAD IS OUT\n");
  return NULL;
}

void *StopServer(void *ip) {
  int *ip_addres = ip;
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
  server_settings.sin_addr.s_addr = *ip_addres;
  server_settings.sin_port = htons(PORT);

  char buff[SIZE_BUFF] = {0};
  strcpy(buff, "close");
  sendto(main_sfd, buff, SIZE_BUFF, 0, (SA *)&server_settings, sizeof(server_settings));

  return NULL;
}

int main() {
  pthread_t arr_treads[POOL_TREADS] = {0};

  int ip_addres = 0;
  inet_pton(AF_INET, IP_ADDRES, &ip_addres);
  int main_sfd = socket(AF_INET, SOCK_DGRAM, 0);

  if (main_sfd == -1) {
    handler_error("socket");
  }

  struct sockaddr_in server_settings, client_settings;
  server_settings.sin_family = AF_INET;
  server_settings.sin_addr.s_addr = ip_addres;
  server_settings.sin_port = htons(PORT);

  if (bind(main_sfd, (SA *)&server_settings, sizeof(server_settings)) == -1) {
    handler_error("bind");
  }

  socklen_t size_len_serv = sizeof(struct sockaddr_in);
  socklen_t size_len_client = sizeof(struct sockaddr_in);
  pthread_t stop_tread;
  pthread_create(&stop_tread, NULL, StopServer, (void *)&ip_addres);
  int ports[POOL_TREADS] = {0};
  for (int i = 0; i < POOL_TREADS; i++) {
    ports[i] = PORT + i + 1;
    pthread_create(&arr_treads[i], NULL, ChildServer, &ports[i]);
  }

  printf("SERVER START WORK\n");
  printf("PRESS 0 (ZERO) SERVER STOP\n");
  int recv_bytes = 0;
  int send_bytes = 0;
  char buff[SIZE_BUFF] = {0};
  int i = 0;
  while (stop) {
    recvfrom(main_sfd, buff, SIZE_BUFF, 0, (SA *)&client_settings, &size_len_client);
    if(recv_bytes <= 0){
      handler_error("recvfrom");
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
    sendto(main_sfd, buff, SIZE_BUFF, 0, (SA *)&client_settings, size_len_client);
  }
  pthread_join(stop_tread, NULL);
  for (int i = 0; i < POOL_TREADS; i++) {
    pthread_join(arr_treads[i], NULL);
  }
  close(main_sfd);
  printf("SERVER END WORK\n");
  exit(EXIT_SUCCESS);
}
