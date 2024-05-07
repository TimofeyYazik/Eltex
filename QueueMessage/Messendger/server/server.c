#include "thread/thread.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static inline void _InitController(Controller *cont, NameList *list, MessageStorage *storage, DsList *ds_list){
  ds_list->len = 0;
  ds_list->size = 10;
  ds_list->ds = malloc(sizeof(mqd_t) * ds_list->size);

  list->len = 0;
  list->size = 10;
  list->name = malloc(sizeof(char*) * list->size);

  storage->len = 0;
  storage->size = 50;
  storage->msg = malloc(sizeof(Message) * storage->size);

  cont->stop_server = 1;

  cont->list = list;
  cont->storage = storage; 
  cont->ds_list = ds_list;
}

int main(){
  pthread_t thread_receive;
  pthread_t thread_register;
  pthread_t thread_send;
  pthread_t thread_stop;
  DsList ds_list;
  NameList list;
  MessageStorage storage;
  Controller cont;
  _InitController(&cont, &list, &storage, &ds_list);
  pthread_create(&thread_stop, NULL, ThreadStop, (void *)&cont);
  pthread_create(&thread_register, NULL, ThreadRegisterClient, (void *)&cont);
  pthread_create(&thread_send, NULL, ThreadSendClient, (void *)&cont);
  pthread_create(&thread_receive, NULL, ThreadReceiveClient, (void *)&cont);
  while(cont.stop_server);
  pthread_join(thread_send, NULL);
  pthread_join(thread_receive, NULL);
  pthread_join(thread_register, NULL);
  pthread_join(thread_stop, NULL);
  free(storage.msg);
  for(int i = 0; i < list.len; i++){
    free(list.name[i]);
  }
  free(list.name);  
  free(ds_list.ds);
  exit(EXIT_SUCCESS);
}


