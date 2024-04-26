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

volatile char stop_server = 1;

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
  while(stop_server) {
    if(flag_len != list->len) {
      fprintf(stderr, "check name: %s\n", list->name[flag_len]);
      ds_list->ds[flag_len] = mq_open(list->name[flag_len], O_CREAT | O_WRONLY, S_IWUSR | S_IRUSR, &attr);
      if (ds_list->ds[flag_len] == -1) {
        fprintf(stderr, "ThreadReceiveClient mq_open failed with error: %d\n", errno);
        perror("mq_open");
      }
      ds_list->len++;
      if (ds_list->len == ds_list->size) {
        ds_list->size *= 2 - (ds_list->size / 2);
        ds_list->ds = realloc(ds_list->ds, sizeof(mqd_t) * ds_list->size);
      }
      flag_len = list->len;
      for (int i = 0; i < ds_list->len; i++) {
        for(int j = 0; j < storage.len; j++) {
          if(mq_send(ds_list->ds[i], (char*)&storage.msg[j], sizeof(Message), 0) == -1) perror("ThreadReceiveClient mq_send");
        }
      }
    }
    usleep(10000);
  }
  for (int i = 0; i < ds_list->len; i++) {
    mq_close(ds_list->ds[i]);
  }
  free(ds_list->ds);
}

void *ThreadReceiveClient(void *arg){
  fprintf(stderr, "ThreadReceiveClient start\n");
  Message msg_buf = {0};
  storage.size = 50;
  storage.msg = malloc(sizeof(Message) * storage.size);
  struct mq_attr attr;
  attr.mq_flags = 0;
  attr.mq_maxmsg = 50;
  attr.mq_msgsize = sizeof(Message);
  attr.mq_curmsgs = 0;
  mqd_t ds_queue_server = mq_open(NAME_QUEUE_SERVER, O_CREAT | O_RDONLY, S_IWUSR | S_IRUSR, &attr);
  if (ds_queue_server == -1) {
    fprintf(stderr, "ThreadReceiveClient mq_open failed with error: %d\n", errno);
    perror("mq_open");
    exit(EXIT_FAILURE);
  }
  while(stop_server) {
      mq_receive(ds_queue_server, (char*)&msg_buf, sizeof(Message), NULL);
      storage.msg[storage.len] = msg_buf;
      fprintf(stderr ,"ThreadReceiveClient check: %s\n", storage.msg[storage.len].text);
      storage.len++;
      if (storage.len == storage.size) {
        storage.size *= 2 - (storage.size / 2);
        storage.msg = realloc(storage.msg, sizeof(Message) * storage.size);
      }
      usleep(10000);
      memset(msg_buf.text, 0, sizeof(msg_buf.text));
      memset(msg_buf.name, 0, sizeof(msg_buf.name));
  }
  mq_close(ds_queue_server);
  mq_unlink(NAME_QUEUE_SERVER);
  exit(EXIT_SUCCESS);
}

void *ThreadRegisterClient(void *arg){
  char status_ok[MAX_NAME_LEN] = GOOD_STATUS;
  char status_error[MAX_NAME_LEN] = BAD_STATUS;
  fprintf(stderr, "ThreadRegisterClient start\n");
  NameList *list = (NameList*)arg;
  struct mq_attr attr;
  attr.mq_flags = 0;
  attr.mq_maxmsg = 50;
  attr.mq_msgsize = MAX_NAME_LEN;
  attr.mq_curmsgs = 0;
  list->name[0] = malloc(sizeof(char) * MAX_NAME_LEN);
  list->len = 0;
  char request_name[MAX_NAME_LEN] = {0};
  mqd_t ds_queue_register = mq_open(NAME_QUEUE_REGISTER, O_CREAT | O_RDWR, S_IWUSR | S_IRUSR, &attr);
  if (ds_queue_register == -1) {
    fprintf(stderr, "ThreadRegisterClient mq_open failed with error: %d\n", errno);
    perror("mq_open");
    exit(EXIT_FAILURE);
  }
  while(stop_server) {
    if(mq_receive(ds_queue_register, request_name, MAX_NAME_LEN, NULL) == -1) {
      fprintf(stderr, "ThreadRegisterClient mq_receive failed with error: %d\n", errno);
      perror("mq_receive");
      exit(EXIT_FAILURE);
    }
    fprintf(stderr, "check name: %s\n", request_name);
    for(int i = 0; i < list->len; i++) {
      if (strcmp(list->name[i], request_name) == 0) {
        fprintf(stderr, "check status: %s\n", status_error);
        mq_send(ds_queue_register, status_error, MAX_NAME_LEN, 0);
        break;
      }
      if (i == list->len - 1) {
        list->name[list->len] = malloc(sizeof(char) * MAX_NAME_LEN);
        strcpy(list->name[list->len], request_name);
        fprintf(stderr, "check status: %s\n", status_ok);
        mq_send(ds_queue_register, status_ok, MAX_NAME_LEN, 0);
        list->len++;
        if(list->len == list->size) {
        list->size *= 2 - (list->size / 2);
        list->name = realloc(list->name, sizeof(char*) * list->size);
      }
        break;
      }
    }
    if(list->len == 0){
      fprintf(stderr, "check status: %s\n", status_ok);
      mq_send(ds_queue_register, status_ok, MAX_NAME_LEN, 0);
      list->len++;
    }
    memset(request_name, 0, MAX_NAME_LEN);
    sleep(1);
  }
  mq_close(ds_queue_register);
  mq_unlink(NAME_QUEUE_REGISTER);
  exit(EXIT_SUCCESS);
}

// void *ThreadStop(void *arg){
//   char stop[5] = {0};
//   fprintf(stderr, "ThreadStop start\n");
//   while (stop_server)
//   {
//     scanf("%d", &stop);
//   } 
// }

int main(){
  mq_unlink("/lp");
  mq_unlink("/kl");
  mq_unlink("/hi");
  mq_unlink("/j");
  mq_unlink("/l");
  mq_unlink("/nigger");
  mq_unlink("/user");
  mq_unlink("/hi");
  mq_unlink(NAME_QUEUE_SERVER);
  mq_unlink(NAME_QUEUE_REGISTER);
  pthread_t thread_receive;
  pthread_t thread_register;
  pthread_t thread_send;
  pthread_t thread_stop;
  NameList list;
  list.len = 0;
  list.size = 10;
  list.name = malloc(sizeof(char*) * list.size);
  // pthread_create(&thread_stop, NULL, ThreadStop, NULL);
  pthread_create(&thread_register, NULL, ThreadRegisterClient, (void *)&list);
  pthread_create(&thread_send, NULL, ThreadSendClient, (void *)&list);
  pthread_create(&thread_receive, NULL, ThreadReceiveClient, (void *)&list);
  sleep(1215752192);

  pthread_join(thread_send, NULL);
  pthread_join(thread_receive, NULL);
  pthread_join(thread_register, NULL);
  
  exit(EXIT_SUCCESS);
}