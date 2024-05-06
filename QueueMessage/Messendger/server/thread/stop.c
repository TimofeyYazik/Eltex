#include "thread.h"


void *ThreadStop(void *arg){
  Controller *cont = (Controller*)arg;
  struct mq_attr attr;
  attr.mq_flags = 0;
  attr.mq_maxmsg = 50;
  attr.mq_msgsize = MAX_NAME_LEN;
  attr.mq_curmsgs = 0;

  struct mq_attr attr1;
  attr1.mq_flags = 0;
  attr1.mq_maxmsg = 50;
  attr1.mq_msgsize = sizeof(Message);
  attr1.mq_curmsgs = 0;
  fprintf(stderr, "ThreadStop start\n");
  char stop_regis[MAX_NAME_LEN] = {0};
  Message stop_receive = {0};
  while (cont->stop_server) {
    scanf("%d", &cont->stop_server);
  } 
  mqd_t ds_queue_server = mq_open(NAME_QUEUE_SERVER, O_CREAT | O_WRONLY, S_IWUSR | S_IRUSR, &attr1);
  if(ds_queue_server == -1) {
    fprintf(stderr, "ThreadStop mq_open failed with error: %d\n", errno);
    perror("mq_open");
    return NULL;
  }
  mqd_t ds_queue_register = mq_open(NAME_QUEUE_REGISTER, O_CREAT | O_WRONLY, S_IWUSR | S_IRUSR, &attr);
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
}