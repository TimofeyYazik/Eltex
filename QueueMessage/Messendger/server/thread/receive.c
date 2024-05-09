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
  mqd_t ds_queue_register = mq_open(NAME_QUEUE_REGISTER, O_CREAT | O_WRONLY, mode_mqueue, &attr);
  mqd_t ds_queue_server = mq_open(NAME_QUEUE_SERVER, O_CREAT | O_RDONLY, mode_mqueue, &attr);
  while(cont->stop_server) {
    mq_receive(ds_queue_server, (char*)&msg_buf, sizeof(Message), NULL);
    if(msg_buf.status == IS_ONLINE){
      MsgCopy(&storage->msg[storage->len], &msg_buf);
      fprintf(stderr, "ThreadReceiveClient check: text = %s len = %d status = %d\n", storage->msg[storage->len].text, storage->len, storage->msg[storage->len].status);
      storage->len++;
      if (storage->len == storage->size) StorageMemRealloc(storage);
    }
    if(msg_buf.status == IS_SHOTDOWN) break;
    if(msg_buf.status == IS_OUT){
      Message request = {0};
      fprintf(stderr, "ThreadReceiveClient check name is out: %s\n", msg_buf.name);
      request.status = IS_SERVER_MESSAGE;
      sprintf(request.text, "client is out: %s", msg_buf.name);
      strcpy(request.name, msg_buf.name);
      MsgCopy(&storage->msg[storage->len], &request);
      storage->len++;
      if (storage->len == storage->size) StorageMemRealloc(storage);
      for(int i = 0; i < list->len; i++) {
        if(strcmp(list->name[i], msg_buf.name) == 0){
          fprintf(stderr, "SHIFT\n");
          ShiftDsList(cont->ds_list, i);
          // mq_unlink(list->name[i]);
          ShiftList(list, i);
          break;
        }
      }
    }
    if(msg_buf.status == IS_REG){
      Message request = {0};
      int i = 0;
      for(i = 0; i < list->len; i++) {
        if (strcmp(list->name[i], msg_buf.name) == 0) {
          request.status = BAD_STATUS;
          mq_send(ds_queue_register, (char*)&request, sizeof(Message), 0);
          break;
        }
      }
      if(i == list->len) {
        request.status = GOOD_STATUS;
        mq_send(ds_queue_register, (char*)&request, sizeof(Message), 0);
        request.status = IS_SERVER_MESSAGE;
        strcpy(request.name, msg_buf.name);
        sprintf(request.text, "new client: %s", msg_buf.name);
        MsgCopy(&storage->msg[storage->len], &request);
        printf("ThreadReceiveClient IMPORTANT check: text = %s len = %d status = %d\n", storage->msg[storage->len].text, storage->len, storage->msg[storage->len].status);
        storage->len++;
        if (storage->len == storage->size) StorageMemRealloc(storage);
        if(list->name[list->len] == NULL) list->name[list->len] = malloc(sizeof(char) * MAX_NAME_LEN);
        strcpy(list->name[list->len], msg_buf.name);
        list->len++;
        if(list->len == list->size) ListMemRealloc(list);
      }
    }
    usleep(10000);
  }
  mq_close(ds_queue_server);
  mq_close(ds_queue_register);
  // for(int i = 0; i < list->size; i++) mq_unlink(list->name[i]);
  mq_unlink(NAME_QUEUE_REGISTER);
  mq_unlink(NAME_QUEUE_SERVER);
  printf("ThreadReceiveClient end\n");
}
