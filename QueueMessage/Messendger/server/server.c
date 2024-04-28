#include "thread.h"

MessageStorage storage = {0};
volatile int stop_server = 1;

int main(){
  mq_unlink("/lp");
  mq_unlink("/kl");
  mq_unlink("/hi");
  mq_unlink("/j");
  mq_unlink("/l");
  mq_unlink("/nigger");
  mq_unlink("/user");
  mq_unlink("/hi");
  mq_unlink("/niger");
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
  pthread_create(&thread_stop, NULL, ThreadStop, NULL);
  pthread_create(&thread_register, NULL, ThreadRegisterClient, (void *)&list);
  pthread_create(&thread_send, NULL, ThreadSendClient, (void *)&list);
  pthread_create(&thread_receive, NULL, ThreadReceiveClient, (void *)&list);
  while(stop_server);
  pthread_join(thread_send, NULL);
  pthread_join(thread_receive, NULL);
  pthread_join(thread_register, NULL);
  pthread_join(thread_stop, NULL);
  free(storage.msg);
  for(int i = 0; i < list.len; i++){
    free(list.name[i]);
  }
  exit(EXIT_SUCCESS);
}