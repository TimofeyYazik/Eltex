#include "thread.h"

void *ThreadSendClient(void *arg){
  fprintf(stderr, "ThreadSendClient start\n");
  mode_t mode_mqueue = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
  Controller *cont = (Controller*)arg;
  NameList *list = cont->list;
  MessageStorage *storage = cont->storage;
  DsList *ds_list = cont->ds_list;
  struct mq_attr attr;
  InitAttr(&attr, sizeof(Message));
  int list_len = 0;
  int storage_len = 0;
  while(cont->stop_server) {
    if(list_len > list->len) { 
      list_len = list->len;
    }
    if(list_len < list->len) {
      fprintf(stderr, "ThreadSendClient check name: list_len = %d list->len =%d %s\n", list_len, list->len, list->name[list_len]);
      ds_list->ds[list_len] = mq_open(list->name[list_len], O_WRONLY, mode_mqueue, &attr);
      if (ds_list->ds[list_len] == -1) {
        fprintf(stderr, "ThreadSendClient mq_open failed with error: %d\n", errno);
        perror("mq_open");
      }
      ds_list->len++;
      if (ds_list->len == ds_list->size) DsListMemRealloc(ds_list);
      for (int i = list_len; i < ds_list->len; i++) {
        for(int j = 0; j < storage->len; j++) {
          if(mq_send(ds_list->ds[i], (char*)&storage->msg[j], sizeof(Message), 0) == -1) 
            perror("ThreadReceiveClient mq_send");
        }
      }
      list_len = list->len;
    }
    if(storage->len != storage_len) {
      printf("ThreadSendClient check: storage->len = %d storage_len = %d\n", storage->len, storage_len);
      for (int i = 0; i < ds_list->len; i++) {
        for(int j = storage_len; j < storage->len; j++) {
          if(mq_send(ds_list->ds[i], (char*)&storage->msg[j], sizeof(Message), 0) == -1) 
            perror("ThreadReceiveClient mq_send");
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
  return NULL;
}