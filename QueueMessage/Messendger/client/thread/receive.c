#include "thread.h"

extern char name[MAX_NAME_LEN];

void *ThreadReceiveServer(void *arg){
  Controller *cont = (Controller*)arg;
  NameList *list = cont->list;
  MessageStorage *storage = cont->storage;
  int size = 0;
  if(storage->msg == NULL) {
    perror("malloc");
    return NULL;
  }
  int x, y;
  int num_message = 0;
  struct mq_attr attr;
  InitAttr(&attr, sizeof(Message));
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

  while (cont->stop_server) {
    MessageWindow(wnd, &storage, (y / 4) * 3);
    if(mq_receive(ds_queue_connect, (char*)&msg, sizeof(Message), NULL) == -1) perror("mq_receive"); 
    MsgCopy(&storage->msg[storage->len], &msg);
    storage->len++;
    if (storage->len == storage->size) {
      storage->size = storage->size * 2 - (storage->size / 2);
      storage->msg = realloc(storage->msg, sizeof(Message) * storage->size);
    }
    usleep(10000);
  }
  delwin(wnd);
  mq_close(ds_queue_connect);
  mq_unlink(name);
}