#include "thread.h"

void *ThreadReceiveClient(void *arg){
  fprintf(stderr, "ThreadReceiveClient start\n");
  mode_t mode_mqueue = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
  Controller *cont = (Controller*)arg;
  MessageStorage *storage = cont->storage;
  NameList *list = cont->list;
  Message msg_buf = {0};
  struct mq_attr attr;
  InitAttr(&attr, sizeof(Message));
  mqd_t ds_queue_server = mq_open(NAME_QUEUE_SERVER, O_CREAT | O_RDONLY, mode_mqueue, &attr);
  if (ds_queue_server == -1) {
    fprintf(stderr, "ThreadReceiveClient mq_open failed with error: %d\n", errno);
    perror("mq_open");
    return NULL;
  }
  while(cont->stop_server) {
    mq_receive(ds_queue_server, (char*)&msg_buf, sizeof(Message), NULL);
    if(!strcmp(msg_buf.text, "/exit")){
      sprintf(msg_buf.text, "client is out: %s", msg_buf.name);
      for(int i = 0; i < list->len; i++) {
        if(strcmp(list->name[i], msg_buf.name) == 0){
          ShiftList(list, i);
          ShiftDsList(cont->ds_list, i);
          break;
        }
      }
      sprintf(msg_buf.name, "/server");
    }
    MsgCopy(&storage->msg[storage->len], &msg_buf);
    fprintf(stderr ,"ThreadReceiveClient check: %s\n", storage->msg[storage->len].text);
    storage->len++;
    if (storage->len == storage->size) StorageMemRealloc(storage);
    usleep(10000);
  }
  mq_close(ds_queue_server);
  mq_unlink(NAME_QUEUE_SERVER);
  printf("ThreadReceiveClient end\n");
}