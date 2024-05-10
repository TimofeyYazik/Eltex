#include "thread.h"

void *ThreadReceiveServer(void *arg){
  mode_t mode_mqueue = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
  ControllerClient *cont = (ControllerClient*)arg;
  NameList *list = cont->list;
  MessageStorage *storage = cont->storage;
  int x, y;
  struct mq_attr attr;
  InitAttr(&attr, sizeof(Message));
  getmaxyx(stdscr, y, x);
  Message msg = {0};
  mqd_t ds_queue_connect = mq_open(cont->name, O_CREAT | O_RDWR, mode_mqueue, &attr);
  if (ds_queue_connect == -1) {
    fprintf(stderr, "ThreadReceiveServer mq_open failed with error: %d\n", errno);
    perror("mq_open");
    return NULL;
  }

  int len_storage = 0;  
  WINDOW *wnd = newwin((y / 4) * 3, (x / 4) * 3, 0, 0);
  box(wnd, 0, 0);
  WINDOW *wnd2 = newwin((y / 4) * 3, (x / 4), 0, (x / 4) * 3);
  box(wnd2, 0, 0);

  while (cont->stop_client) {
    MessageWindow(wnd, storage, (y / 4) * 3);
    UserWindow(wnd2, list);
    mq_receive(ds_queue_connect, (char*)&msg, sizeof(Message), NULL);
    if(msg.status == IS_SERVER_MESSAGE) {
      if(storage->len > 0) {
        if(!strcmp(msg.text, storage->msg[storage->len - 1].text) && strstr(msg.text, "new client:")) {
          continue;
        }  
      }
      if(strstr(msg.text, "new client:")){
        AddNameList(list, msg.name);
        MsgCopy(&storage->msg[storage->len], &msg);
        strcpy(storage->msg[storage->len].name, "server");
        storage->len++;
        if (storage->len == storage->size) StorageMemRealloc(storage);
      }
      if(strstr(msg.text, "client is out:")){
        for (int i = 0; i < list->len; i++) {
          if(strcmp(list->name[i], msg.name) == 0) {
            ShiftList(list, i);
            break;
          }
        }
        MsgCopy(&storage->msg[storage->len], &msg);
        strcpy(storage->msg[storage->len].name, "server");
        storage->len++;
        if (storage->len == storage->size) StorageMemRealloc(storage);
      }
    }
    if(msg.status == IS_SHOTDOWN) break;
    if(msg.status == IS_ONLINE){  
      AddStorageMessege(storage, &msg);
      fprintf(stderr, "ThreadReceiveServer check: text = %s len = %d status = %d\n", storage->msg[storage->len].text, storage->len, storage->msg[storage->len].status);
    }
  }
  unlink(cont->name);
  delwin(wnd);
  mq_close(ds_queue_connect);
  return NULL;
}
