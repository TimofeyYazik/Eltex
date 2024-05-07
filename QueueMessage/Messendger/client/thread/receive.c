#include "thread.h"

void *ThreadReceiveServer(void *arg){
  ControllerClient *cont = (ControllerClient*)arg;
  NameList *list = cont->list;
  MessageStorage *storage = cont->storage;
  int x, y;
  struct mq_attr attr;
  InitAttr(&attr, sizeof(Message));
  getmaxyx(stdscr, y, x);
  Message msg = {0};
  mqd_t ds_queue_connect = mq_open(cont->name, O_CREAT | O_RDONLY, S_IWUSR | S_IRUSR, &attr);
  if (ds_queue_connect == -1) {
    fprintf(stderr, "ThreadReceiveServer mq_open failed with error: %d\n", errno);
    perror("mq_open");
    return NULL;
  }
  int len_storage = 0;  
  WINDOW *wnd = newwin((y / 4) * 3, (x / 4) * 3, 0, 0);
  box(wnd, 0, 0);

  while (cont->stop_client) {
    if(storage->len != len_storage){
      MessageWindow(wnd, storage, (y / 4) * 3);
    }
    mq_receive(ds_queue_connect, (char*)&msg, sizeof(Message), NULL);
    if(strstr(msg.text, "new client:") && !strcmp(msg.text, storage->msg[storage->len - 1].text)) {
      continue;
    }
    MsgCopy(&storage->msg[storage->len], &msg);
    storage->len++;
    if (storage->len == storage->size) {
      storage->size = storage->size * 2 - (storage->size / 2);
      storage->msg = realloc(storage->msg, sizeof(Message) * storage->size);
    }
  }
  delwin(wnd);
  mq_close(ds_queue_connect);
  mq_unlink(cont->name);
  printf("ThreadReceiveServer finished\n");
  return NULL;
}