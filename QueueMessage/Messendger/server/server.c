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
  NameList *list = (NameList*)arg;
  DsList ds_list;
  ds_list->len = 0;
  ds_list->size = 10;
  ds_list->ds = malloc(sizeof(mqd_t) * ds_list.size);
  int flag_len = 0;
  while(1) {
    if(flag_len != list->len) {
    for (int i = list->len - flag_len; i < list->len; i++) {
      ds_list->ds[ds_list.len] = mq_open(list->name[i], O_CREAT | O_WRONLY, S_IWUSR | S_IRUSR, &attr);
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
        mq_send(ds_list.ds[i], storage.msg[j], sizeof(Message), 0);
      }
    }
    usleep(10000);
  }
  exit(EXIT_SUCCESS);
}

void *ThreadReceiveClient(void *arg){
  struct mq_attr attr;
  attr.mq_flags = 0;
  attr.mq_maxmsg = 50;
  attr.mq_msgsize = sizeof(Message);
  attr.mq_curmsgs = 0;
  mqd_t ds_queue_server = mq_open(NAME_QUEUE_SERVER, O_CREAT | O_RDWR, S_IWUSR | S_IRUSR, &attr);
  storage.size = 50;
  storage.msg = malloc(sizeof(Message) * storage.size);
  while(1) {
      mq_receive(ds_queue_server, storage.msg + storage.len, sizeof(Message), NULL);
      storage.len++;
      if (storage.len == storage.size) {
        storage.size *= 2 - (storage.size / 2);
        storage.msg = realloc(storage.msg, sizeof(Message) * storage.size);
      }
    }
    usleep(10000);
  }
  mq_close(ds_queue_server);
  mq_unlink(NAME_QUEUE_SERVER);
  exit(EXIT_SUCCESS);
}

void *ThreadRegisterClient(void *arg){
  NameList *list = (NameList*)arg;
  struct mq_attr attr;
  attr.mq_flags = 0;
  attr.mq_maxmsg = 50;
  attr.mq_msgsize = sizeof(Message);
  attr.mq_curmsgs = 0;
  list->len = 1;
  list->name[0] = malloc(sizeof(char) * MAX_NAME_LEN);
  char request_name[MAX_NAME_LEN] = {0};
  mqd_t ds_queue_register = mq_open(NAME_QUEUE_REGISTER, O_CREAT | O_RDWR, S_IWUSR | S_IRUSR, &attr);
  while(1) {
    mq_receive(ds_queue_register, request_name, MAX_NAME_LEN, NULL);
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
  pthread_create(&thread_register, &list, ThreadRegisterClient, NULL);
  pthread_create(&thread_send, &list, ThreadSendClient, NULL);
  pthread_create(&thread_receive, &list, ThreadReceiveClient, NULL);

  pthread_join(thread_send, NULL);
  pthread_join(thread_receive, NULL);
  pthread_join(thread_register, NULL);
  
  exit(EXIT_SUCCESS);
}