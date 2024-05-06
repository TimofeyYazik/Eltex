#include "thread.h"

void *ThreadSendClient(void *arg){
  fprintf(stderr, "ThreadSendClient start\n");
  Controller *cont = (Controller*)arg;
  NameList *list = cont->list;
  MessageStorage *storage = cont->storage;
  DsList *ds_list = malloc(sizeof(DsList));
  ds_list->len = 0;
  ds_list->size = 10;
  ds_list->ds = malloc(sizeof(mqd_t) * ds_list->size);
  int flag_len = 0;
  struct mq_attr attr;
  attr.mq_flags = 0;
  attr.mq_maxmsg = 50;
  attr.mq_msgsize = sizeof(Message);
  attr.mq_curmsgs = 0;
  int storage_len = storage->len;
  while(cont->stop_server) {
    if(flag_len != list->len) {
      fprintf(stderr, "ThreadSendClient check name: flag_len = %d list->len =%d %s\n", flag_len, list->len, list->name[flag_len]);
      ds_list->ds[flag_len] = mq_open(list->name[flag_len], O_CREAT | O_WRONLY, S_IWUSR | S_IRUSR, &attr);
      if (ds_list->ds[flag_len] == -1) {
        fprintf(stderr, "ThreadSendClient mq_open failed with error: %d\n", errno);
        perror("mq_open");
      }
      ds_list->len++;
      if (ds_list->len == ds_list->size) {
        ds_list->size = 2 * ds_list->size - (ds_list->size / 2);
        ds_list->ds = realloc(ds_list->ds, sizeof(mqd_t) * ds_list->size);
      }
      for (int i = flag_len; i < ds_list->len; i++) {
        for(int j = 0; j < storage->len; j++) {
          if(mq_send(ds_list->ds[i], (char*)&storage->msg[j], sizeof(Message), 0) == -1) 
          perror("ThreadReceiveClient mq_send");
        }
      }
      flag_len = list->len;
    }
    if(storage->len != storage_len) {
    for (int i = 0; i < ds_list->len; i++) {
      for(int j = storage_len; j < storage->len; j++) {
        if(mq_send(ds_list->ds[i], (char*)&storage->msg[j], sizeof(Message), 0) == -1) perror("ThreadReceiveClient mq_send");
      }
    }
    storage_len = storage->len; 
    }
    usleep(10000);
  }
  for (int i = 0; i < ds_list->len; i++) {
    mq_close(ds_list->ds[i]);
  }
  fprintf(stderr, "ThreadSendClient end\n");
  free(ds_list->ds);
}