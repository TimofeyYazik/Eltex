#include "thread.h"

extern char name[MAX_NAME_LEN];
extern MessageStorage storage;
extern int stop_client;

void *ThreadReceiveServer(void *arg){
  int size = 0;
  storage.len = 0;
  storage.size = 50;
  storage.msg = malloc(sizeof(Message) * storage.size);
  if(storage.msg == NULL) {
    perror("malloc");
    return NULL;
  }
  int x, y;
  int num_message = 0;
  struct mq_attr attr;
  attr.mq_flags = 0;
  attr.mq_maxmsg = 50;
  attr.mq_msgsize = sizeof(Message);
  attr.mq_curmsgs = 0;
  getmaxyx(stdscr, y, x);
  Message msg = {0};
  mqd_t ds_queue_connect = mq_open(name, O_CREAT | O_RDONLY, S_IWUSR | S_IRUSR, &attr);
  if (ds_queue_connect == -1) {
    fprintf(stderr, "ThreadReceiveServer mq_open failed with error: %d\n", errno);
    perror("mq_open");
    return NULL;
  }

  WINDOW *wnd = newwin((y / 4) * 3, (x / 4) * 3, 0, 0);
  box(wnd, 0, 0);

  while (stop_client) {
    MessageWindow(wnd, &storage, (y / 4) * 3);
    if(mq_receive(ds_queue_connect, (char*)&msg, sizeof(Message), NULL) == -1) perror("mq_receive"); 
    MsgCopy(&storage.msg[storage.len], &msg);
    storage.len++;
    if (storage.len == storage.size) {
      storage.size = storage.size * 2 - (storage.size / 2);
      storage.msg = realloc(storage.msg, sizeof(Message) * storage.size);
    }
    usleep(10000);
  }
  delwin(wnd);
  mq_close(ds_queue_connect);
  mq_unlink(name);
}