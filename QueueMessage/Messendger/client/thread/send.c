#include "thread.h"

void *ThreadSendServer(void *arg){
  mode_t mode_mqueue = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
  ControllerClient *cont = (ControllerClient*)arg;
  NameList *list = cont->list;
  MessageStorage *storage = cont->storage;
  int x, y;
  getmaxyx(stdscr, y, x);
  WINDOW *wnd = newwin(y / 4, x, (y / 4) * 3, 0);
  struct mq_attr attr;
  Message msg = {0};
  strcpy(msg.name, cont->name);
  InitAttr(&attr, sizeof(Message));
  mqd_t ds_queue_server = mq_open(NAME_QUEUE_SERVER, O_CREAT | O_WRONLY, mode_mqueue, &attr);
  if(ds_queue_server == -1) {
    fprintf(stderr, "ThreadSendServer mq_open failed with error: %d\n", errno);
    perror("mq_open");
    return NULL;
  }
  while (cont->stop_client) {
    InputMessageWindow(wnd, &msg);
    if (!strcmp(msg.text, "/exit")) {
      cont->stop_client = 0;  
      mq_send(ds_queue_server, (char *)&msg, sizeof(Message), 0);
      break;
    }
    if(mq_send(ds_queue_server, (char *)&msg, sizeof(Message), 0) == -1){
      perror("mq_send");
    }
  }
  mq_close(ds_queue_server);
  delwin(wnd);
  return NULL;
}