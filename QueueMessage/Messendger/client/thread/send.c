#include "thread.h"

extern pthread_mutex_t mutex;

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
  mqd_t ds_queue_server = mq_open(NAME_QUEUE_SERVER, O_WRONLY, mode_mqueue, &attr);
  if(ds_queue_server == -1) {
    fprintf(stderr, "ThreadSendServer mq_open failed with error: %d\n", errno);
    perror("mq_open");
    return NULL;
  }
  while (cont->stop_client) {
    // pthread_mutex_lock(&mutex);
    InputMessageWindow(wnd, &msg);
    // pthread_mutex_unlock(&mutex);
    if (!strcmp(msg.text, "/exit")) {
      cont->stop_client = 0;  
      msg.status = IS_OUT;
      mq_send(ds_queue_server, (char *)&msg, sizeof(Message), 0);
      break;
    }
    msg.status = IS_ONLINE;
    if(mq_send(ds_queue_server, (char *)&msg, sizeof(Message), 0) == -1){
      perror("mq_send");
    }
  }
  mq_close(ds_queue_server);
  delwin(wnd);
  return NULL;
}