#include "thread.h"

extern char name[MAX_NAME_LEN];
extern MessageStorage storage;
extern int stop_client;

void MsgCopy(Message *dst, Message *src){
  strcpy(dst->name, src->name);
  strcpy(dst->text, src->text);
}

void *ThreadSendServer(void *arg){
  int x, y;
  getmaxyx(stdscr, y, x);
  WINDOW *wnd = newwin(y / 4, x, (y / 4) * 3, 0);
  struct mq_attr attr;
  Message msg = {0};
  strcpy(msg.name, name);
  attr.mq_flags = 0;
  attr.mq_maxmsg = 50;
  attr.mq_msgsize = sizeof(Message);
  attr.mq_curmsgs = 0;
  mqd_t ds_queue_receive = mq_open(name, O_CREAT | O_WRONLY, S_IWUSR | S_IRUSR, &attr);
  if(ds_queue_receive == -1) {
    fprintf(stderr, "ThreadSendServer mq_open failed with error: %d\n", errno);
    perror("mq_open");
    return NULL;
  }
  mqd_t ds_queue_server = mq_open(NAME_QUEUE_SERVER, O_CREAT | O_WRONLY, S_IWUSR | S_IRUSR, &attr);
  if(ds_queue_server == -1) {
    fprintf(stderr, "ThreadSendServer mq_open failed with error: %d\n", errno);
    perror("mq_open");
    mq_close(ds_queue_receive);
    return NULL;
  }
  while (stop_client) {
    InputMessageWindow(wnd, &msg);
    if (!strcmp(msg.text, "/exit")) {
      stop_client = 0;  
      mq_send(ds_queue_server, (char *)&msg, sizeof(Message), 0);
      break;
    }
    if(mq_send(ds_queue_server, (char *)&msg, sizeof(Message), 0) == -1){
      perror("mq_send");
    }
  }
  mq_close(ds_queue_server);
  mq_close(ds_queue_receive);
  delwin(wnd);
}