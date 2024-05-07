#include "thread.h"


  // int flags = O_RDWR | O_CREAT | O_NONBLOCK;
  // mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

void *ThreadSendClient(void *arg){
  fprintf(stderr, "ThreadSendClient start\n");
  Controller *cont = (Controller*)arg;
  NameList *list = cont->list;
  MessageStorage *storage = cont->storage;
  DsList *ds_list = cont->ds_list;
  struct mq_attr attr;
  InitAttr(&attr, sizeof(Message));
  int list_len = list->len;
  int storage_len = storage->len;
  while(cont->stop_server) {
    if(list_len > list->len) { 
      list_len = list->len;
    }
    if(list_len < list->len) {
      fprintf(stderr, "ThreadSendClient check name: list_len = %d list->len =%d %s\n", list_len, list->len, list->name[list_len]);
      ds_list->ds[list_len] = mq_open(list->name[list_len], O_CREAT | O_WRONLY, S_IWUSR | S_IRUSR, &attr);
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
      for (int i = 0; i < ds_list->len; i++) {
        for(int j = storage_len; j < storage->len; j++) {
          printf("%s %s %d\n", storage->msg[j].name, storage->msg[j].text, j);
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