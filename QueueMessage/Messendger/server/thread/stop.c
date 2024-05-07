#include "thread.h"


void *ThreadStop(void *arg){
  Controller *cont = (Controller*)arg;
  mode_t mode_mqueue = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
  struct mq_attr attr;
  InitAttr(&attr, sizeof(Message));
  fprintf(stderr, "ThreadStop start\n");
  Message stop_server = {0};
  stop_server.status = IS_SHOTDOWN;
  while (cont->stop_server) {
    scanf("%d", &cont->stop_server);
  } 
  mqd_t ds_queue_server = mq_open(NAME_QUEUE_SERVER, O_CREAT | O_WRONLY, mode_mqueue, &attr);
  if(ds_queue_server == -1) {
    fprintf(stderr, "ThreadStop mq_open failed with error: %d\n", errno);
    perror("mq_open");
    return NULL;
  }
  mq_send(ds_queue_server, (char*)&stop_server, sizeof(Message), 0);
  mq_close(ds_queue_server);
  printf("ThreadStop exit\n");
  return NULL;
}