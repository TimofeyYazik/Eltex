#include "thread.h"

extern pthread_mutex_t mutex;

void *ThreadReceiveServer(void *arg){
  mode_t mode_mqueue = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
  FILE *f = fopen("test", "w");
  ControllerClient *cont = (ControllerClient*)arg;
  NameList *list = cont->list;
  MessageStorage *storage = cont->storage;
  int x, y;
  struct mq_attr attr;
  InitAttr(&attr, sizeof(Message));
  getmaxyx(stdscr, y, x);
  Message msg = {0};
  mqd_t ds_queue_connect = mq_open(cont->name, O_CREAT | O_RDONLY, mode_mqueue, &attr);
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
      pthread_mutex_lock(&mutex);
      MessageWindow(wnd, storage, (y / 4) * 3);
      pthread_mutex_unlock(&mutex);
    }
    mq_receive(ds_queue_connect, (char*)&msg, sizeof(Message), NULL);
    fprintf(f, "%s\n", msg.text);
    if(msg.status == IS_SERVER_MESSAGE) {
      if(strstr(msg.text, "new client:")){
        list->name[list->len] = malloc(sizeof(char) * MAX_NAME_LEN);
        strcpy(list->name[list->len], msg.name);
        list->len++;
        if(list->len == list->size) ListMemRealloc(list);
      }
      if(strstr(msg.text, "client is out: ")){
        for (int i = 0; i < list->len; i++) {
          if(strcmp(list->name[i], msg.name) == 0) {
            ShiftList(list, i);
            break;
          }
        }
      }
      pthread_mutex_lock(&mutex);
      UserWindow(wnd, list);
      pthread_mutex_unlock(&mutex);
    }
    if(msg.status == IS_SHOTDOWN) break;
    if(msg.status == IS_ONLINE){  
      MsgCopy(&storage->msg[storage->len], &msg);
      storage->len++;
      if (storage->len == storage->size) StorageMemRealloc(storage);
    }
  }
  delwin(wnd);
  fclose(f);
  mq_close(ds_queue_connect);
  mq_unlink(cont->name);
  return NULL;
}