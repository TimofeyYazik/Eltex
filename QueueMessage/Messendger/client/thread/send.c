#include "thread.h"

extern char name[MAX_NAME_LEN];

void *ThreadSendServer(void *arg){
  Controller *cont = (Controller*)arg;
  NameList *list = cont->list;
  MessageStorage *storage = cont->storage;
  int x, y;
  getmaxyx(stdscr, y, x);
  WINDOW *wnd = newwin(y / 4, x, (y / 4) * 3, 0);
  struct mq_attr attr;
  Message msg = {0};
  strcpy(msg.name, name);
  InitAttr(&attr, sizeof(Message));
  mqd_t ds_queue_server = mq_open(NAME_QUEUE_SERVER, O_CREAT | O_WRONLY | O_NONBLOCK, S_IWUSR | S_IRUSR, &attr);
  if(ds_queue_server == -1) {
    fprintf(stderr, "ThreadSendServer mq_open failed with error: %d\n", errno);
    perror("mq_open");
    return NULL;
  }
  while (cont->stop_server) {
    InputMessageWindow(wnd, &msg);
    if (!strcmp(msg.text, "/exit")) {
      cont->stop_server = 0;  
      mq_send(ds_queue_server, (char *)&msg, sizeof(Message), 0);
      break;
    }
    if(mq_send(ds_queue_server, (char *)&msg, sizeof(Message), 0) == -1){
      perror("mq_send");
    }
  }
  mq_close(ds_queue_server);
  delwin(wnd);
  printf("ThreadSendServer end\n");
  return NULL;
}