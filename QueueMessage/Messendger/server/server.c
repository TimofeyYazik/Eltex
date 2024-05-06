#include "thread/thread.h"

int main(){
  pthread_t thread_receive;
  pthread_t thread_register;
  pthread_t thread_send;
  pthread_t thread_stop;
  NameList list;
  list.len = 0;
  list.size = 10;
  list.name = malloc(sizeof(char*) * list.size);
  MessageStorage storage;
  storage.len = 0;
  storage.size = 50;
  storage.msg = malloc(sizeof(Message) * storage.size);
  Controller cont;
  cont.stop_server = 1;
  cont.list = &list;
  cont.storage = &storage;
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
  exit(EXIT_SUCCESS);
}