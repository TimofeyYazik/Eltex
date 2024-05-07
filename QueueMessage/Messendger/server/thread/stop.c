#include "thread.h"


void *ThreadStop(void *arg){
  Controller *cont = (Controller*)arg;
  mode_t mode_mqueue = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
  struct mq_attr attr;
  struct mq_attr attr1;
  InitAttr(&attr, MAX_NAME_LEN);
  InitAttr(&attr1, sizeof(Message));
  fprintf(stderr, "ThreadStop start\n");
  char stop_regis[MAX_NAME_LEN];
  stop_regis[0] = 4;

  Message stop_receive = {0};
  stop_receive.text[0] = 4;
  while (cont->stop_server) {
    scanf("%d", &cont->stop_server);
  } 
  mqd_t ds_queue_server = mq_open(NAME_QUEUE_SERVER, O_CREAT | O_WRONLY, mode_mqueue, &attr1);
  if(ds_queue_server == -1) {
    fprintf(stderr, "ThreadStop mq_open failed with error: %d\n", errno);
    perror("mq_open");
    return NULL;
  }
  mqd_t ds_queue_register = mq_open(NAME_QUEUE_REGISTER, O_CREAT | O_WRONLY, mode_mqueue, &attr);
  if(ds_queue_register == -1) {
    fprintf(stderr, "ThreadStop mq_open failed with error: %d\n", errno);
    perror("mq_open");
    return NULL;
  }
  mq_send(ds_queue_register, stop_regis, MAX_NAME_LEN, 0);
  mq_send(ds_queue_server, (char*)&stop_receive, sizeof(Message), 0);
  mq_close(ds_queue_server);
  mq_close(ds_queue_register);
  printf("ThreadStop exit\n");
  return NULL;
}