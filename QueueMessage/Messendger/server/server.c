#include <mqueue.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>

#include "../custom_type.h"

MessageStorage storage = {0};
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *ThreadSendClient(void *arg){
  fprintf(stderr, "ThreadSendClient start\n");
  NameList *list = (NameList*)arg;
  DsList dlist;
  DsList *ds_list = &dlist;
  ds_list->len = 0;
  ds_list->size = 10;
  ds_list->ds = malloc(sizeof(mqd_t) * ds_list->size);
  int flag_len = 0;
  struct mq_attr attr;
  attr.mq_flags = 0;
  attr.mq_maxmsg = 50;
  attr.mq_msgsize = sizeof(Message);
  attr.mq_curmsgs = 0;
  while(1) {
    if(flag_len != list->len) {
    for (int i = list->len - flag_len; i < list->len; i++) {
      ds_list->ds[ds_list->len] = mq_open(list->name[i], O_CREAT | O_WRONLY, S_IWUSR | S_IRUSR, &attr);
      ds_list->len++;
      if (ds_list->len == ds_list->size) {
        ds_list->size *= 2 - (ds_list->size / 2);
        ds_list->ds = realloc(ds_list->ds, sizeof(mqd_t) * ds_list->size);
      }
    }
    flag_len = list->len;
    }
    for (int i = 0; i < ds_list->len; i++) {
      for(int j = 0; j < storage.len; j++) {
        mq_send(ds_list->ds[i], (char*)&storage.msg[j], sizeof(Message), 0);
      }
    }
    usleep(10000);
  }
  exit(EXIT_SUCCESS);
}

void *ThreadReceiveClient(void *arg){
  fprintf(stderr, "ThreadReceiveClient start\n");
  struct mq_attr attr;
  attr.mq_flags = 0;
  attr.mq_maxmsg = 50;
  attr.mq_msgsize = sizeof(Message);
  attr.mq_curmsgs = 0;
  mqd_t ds_queue_server = mq_open(NAME_QUEUE_SERVER, O_CREAT | O_RDWR, S_IWUSR | S_IRUSR, &attr);
  if (ds_queue_server == -1) {
    fprintf(stderr, "mq_open failed with error: %d\n", errno);
    perror("mq_open");
    exit(EXIT_FAILURE);
  }

  storage.size = 50;
  storage.msg = malloc(sizeof(Message) * storage.size);
  while(1) {
      mq_receive(ds_queue_server, (char*)&storage.msg + storage.len, sizeof(Message), NULL);
      storage.len++;
      if (storage.len == storage.size) {
        storage.size *= 2 - (storage.size / 2);
        storage.msg = realloc(storage.msg, sizeof(Message) * storage.size);
      }
      fprintf(stderr ,"check: %s\n", storage.msg[storage.len - 1].text);
      usleep(10000);
  }
  mq_close(ds_queue_server);
  mq_unlink(NAME_QUEUE_SERVER);
  exit(EXIT_SUCCESS);
}

void *ThreadRegisterClient(void *arg){
  fprintf(stderr, "ThreadRegisterClient start\n");
  NameList *list = (NameList*)arg;
  struct mq_attr attr;
  attr.mq_flags = 0;
  attr.mq_maxmsg = 50;
  attr.mq_msgsize = MAX_NAME_LEN;
  attr.mq_curmsgs = 0;
  list->len = 1;
  list->name[0] = malloc(sizeof(char) * MAX_NAME_LEN);
  char request_name[MAX_NAME_LEN] = {0};
  mqd_t ds_queue_register = mq_open(NAME_QUEUE_REGISTER, O_CREAT | O_RDWR, S_IWUSR | S_IRUSR, &attr);
  if (ds_queue_register == -1) {
    fprintf(stderr, "mq_open failed with error: %d\n", errno);
    perror("mq_open");
    exit(EXIT_FAILURE);
  }
  while(1) {
    if(mq_receive(ds_queue_register, request_name, MAX_NAME_LEN, NULL) == -1) {
      fprintf(stderr, "mq_receive failed with error: %d\n", errno);
      perror("mq_receive");
      exit(EXIT_FAILURE);
    }
    fprintf(stderr, "check: %s\n", request_name);
    for(int i = 0; i < list->len; i++) {
      if (strcmp(list->name[i], request_name) == 0) {
        mq_send(ds_queue_register, BAD_STATUS, strlen(BAD_STATUS) + 1, 0);
        break;
      }
      if (i == list->len - 1) {
        list->name[list->len] = malloc(sizeof(char) * MAX_NAME_LEN);
        strcpy(list->name[list->len], request_name);
        mq_send(ds_queue_register, GOOD_STATUS, strlen(GOOD_STATUS) + 1, 0);
        break;
      }
    }
    list->len++;
    if(list->len == list->size) {
      list->size *= 2 - (list->size / 2);
      list->name = realloc(list->name, sizeof(char*) * list->size);
    }
    memset(request_name, 0, MAX_NAME_LEN);
    usleep(10000);
  }
  mq_close(ds_queue_register);
  mq_unlink(NAME_QUEUE_REGISTER);
  exit(EXIT_SUCCESS);
}

int main(){
  pthread_t thread_receive;
  pthread_t thread_register;
  pthread_t thread_send;
  NameList list;
  list.len = 0;
  list.size = 10;
  list.name = malloc(sizeof(char*) * list.size);
  pthread_create(&thread_register, NULL, ThreadRegisterClient, (void *)&list);
  pthread_create(&thread_send, NULL, ThreadSendClient, (void *)&list);
  pthread_create(&thread_receive, NULL, ThreadReceiveClient, (void *)&list);
  sleep(100000000000);

  pthread_join(thread_send, NULL);
  pthread_join(thread_receive, NULL);
  pthread_join(thread_register, NULL);
  
  exit(EXIT_SUCCESS);
}