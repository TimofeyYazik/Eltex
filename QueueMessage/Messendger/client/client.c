#include "thread/thread.h"

char name[MAX_NAME_LEN] = {0};

static inline void _InitController(Controller *cont, NameList *list, MessageStorage *storage);

int main(){
  NameList list;
  MessageStorage storage;
  Controller cont;
  _InitController(&cont, &list, &storage);
  pthread_t thread_send;
  pthread_t thread_receive;
  pthread_t thread_user;
  Register();
  initscr();
  pthread_create(&thread_send, NULL, ThreadSendServer, (void *)&cont);
  pthread_create(&thread_user, NULL, ThreadUserWindow, (void *)&cont);
  pthread_create(&thread_receive, NULL, ThreadReceiveServer, (void *)&cont);
  while (stop_client);
  endwin();
  pthread_join(thread_send, NULL);
  pthread_join(thread_receive, NULL);
  pthread_join(thread_user, NULL);
  for (int i = 0; i < list.len; i++) {
    free(list.name[i]);
  }
  free(list.name);
  free(storage->msg);
  exit(EXIT_SUCCESS);
}

static inline void _InitController(Controller *cont, NameList *list, MessageStorage *storage){
  list->len = 0;
  list->size = 10;
  list->name = malloc(sizeof(char*) * list.size);

  storage->len = 0;
  storage->size = 50;
  storage->msg = malloc(sizeof(Message) * storage.size);

  cont->stop_server = 1;

  cont->list = list;
  cont->storage = storage;
  cont->ds_list = NULL;
}