#include "thread/thread.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static inline void _InitControllerClient(ControllerClient *cont, NameList *list, MessageStorage *storage);

int main(){
  NameList list;
  MessageStorage storage;
  ControllerClient cont;
  _InitControllerClient(&cont, &list, &storage);
  pthread_t thread_send;
  pthread_t thread_stop;
  pthread_t thread_receive;
  Register(&cont);
  initscr();
  pthread_create(&thread_stop, NULL, ThreadStop, (void *)&cont);
  pthread_create(&thread_send, NULL, ThreadSendServer, (void *)&cont);
  pthread_create(&thread_receive, NULL, ThreadReceiveServer, (void *)&cont);
  pthread_join(thread_stop, NULL);
  pthread_join(thread_send, NULL);
  pthread_join(thread_receive, NULL);
  endwin();
  usleep(1000);
  for (int i = 0; i < list.len; i++) {
    free(list.name[i]);
  }
  free(list.name);
  free(storage.msg);
  pthread_mutex_destroy(&mutex);
  unlink(cont->name);
  exit(EXIT_SUCCESS);
}

static inline void _InitControllerClient(ControllerClient *cont, NameList *list, MessageStorage *storage){
  list->len = 0;
  list->size = 10;
  list->name = malloc(sizeof(char*) * list->size);

  storage->len = 0;
  storage->size = 50;
  storage->msg = malloc(sizeof(Message) * storage->size);

  cont->stop_client = 1;

  cont->list = list;
  cont->storage = storage;
}