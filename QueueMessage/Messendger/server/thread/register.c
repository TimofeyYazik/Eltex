#include <stdio.h>

#include "thread.h"

extern MessageStorage storage;
extern volatile int stop_server;

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
    fprintf(stderr, "ThreadRegisterClient check name: %s\n", request_name);
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
        list->size = 2 * list->size - (list->size / 2);
        list->name = realloc(list->name, sizeof(char*) * list->size);
      }
        break;
      }
    }
    if(list->len == 0){
      fprintf(stderr, "check 0 status: %s\n", status_ok);
      strcpy(list->name[list->len], request_name);
      mq_send(ds_queue_register, status_ok, MAX_NAME_LEN, 0);
      list->len++;
    }
    memset(request_name, 0, MAX_NAME_LEN);
    sleep(1);
  }
  mq_close(ds_queue_register);
  mq_unlink(NAME_QUEUE_REGISTER);
  printf("ThreadRegisterClient exit\n");
}