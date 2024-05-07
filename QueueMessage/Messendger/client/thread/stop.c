#include "thread.h"

void *ThreadStop(void *arg){
  mode_t mode_mqueue = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
  Message stop_receive = {0};
  strcpy(stop_receive.name, "stop");
  strcpy(stop_receive.text, "/exit");
  ControllerClient *cont = (ControllerClient*)arg;
  struct mq_attr attr;
  InitAttr(&attr, sizeof(Message));
  while (1) {
    if(cont->stop_client == 0) {
      mqd_t ds_queue_connect = mq_open(cont->name, O_CREAT | O_WRONLY, mode_mqueue, &attr);
      mq_send(ds_queue_connect, (char*)&stop_receive, sizeof(Message), 0);
      mq_close(ds_queue_connect);
      break;
    }
  }
  return NULL;
}