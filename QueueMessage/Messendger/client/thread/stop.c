#include "thread.h"

void *ThreadStop(void *arg){
  Message stop_receive = {0};
  strcpy(stop_receive.name, "stop");
  strcpy(stop_receive.text, "/exit");
  ControllerClient *cont = (ControllerClient*)arg;
  struct mq_attr attr;
  InitAttr(&attr, sizeof(Message));
  while (1) {
    if(cont->stop_client == 0) {
      mqd_t ds_queue_connect = mq_open(cont->name, O_CREAT | O_RDONLY, S_IWUSR | S_IRUSR, &attr);
      mq_send(ds_queue_connect, (char*)&stop_receive, sizeof(Message), 0);
      break;
    }
  }
  mq_close(cont->ds_queue_connect);
  fprintf(stderr, "ThreadStop end\n");
  return NULL;
}