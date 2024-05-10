#include "thread.h"

void *ThreadReceiveClient(void *arg){
  fprintf(stderr, "ThreadReceiveClient start\n");
  mode_t mode_mqueue = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
  NameList list_copy = {0};
  list_copy.len = 0;
  list_copy.size = 50;
  list_copy.name = malloc(sizeof(char *) * list_copy.size);
  for(int i = 0; i < list_copy.size; i++){
    list_copy.name[i] = NULL;
  }
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
      AddStorageMessege(storage, &msg_buf);
      fprintf(stderr, "ThreadReceiveClient check: text = %s len = %d status = %d\n", storage->msg[storage->len].text, storage->len, storage->msg[storage->len].status);
    }
    if(msg_buf.status == IS_SHOTDOWN) break;
    if(msg_buf.status == IS_OUT){
      Message request = {0};
      fprintf(stderr, "ThreadReceiveClient check name is out: %s\n", msg_buf.name);
      request.status = IS_SERVER_MESSAGE;
      sprintf(request.text, "client is out: %s", msg_buf.name);
      strcpy(request.name, msg_buf.name);
      AddStorageMessege(storage, &request);
      for(int i = 0; i < list->len; i++) {
        if(strcmp(list->name[i], msg_buf.name) == 0){
          ShiftDsList(cont->ds_list, i);
          ShiftList(list, i);
          break;
        }
      }
    }
    if(msg_buf.status == IS_REG){
      Message request = {0};
      int i = 0;
      for(i = 0; i < list_copy.len; i++) {
        if (strcmp(list_copy.name[i], msg_buf.name) == 0) {
          request.status = BAD_STATUS;
          mq_send(ds_queue_register, (char*)&request, sizeof(Message), 0);
          break;
        }
      }
      if(i == list_copy.len) {
        request.status = GOOD_STATUS;
        mq_send(ds_queue_register, (char*)&request, sizeof(Message), 0);
        request.status = IS_SERVER_MESSAGE;
        strcpy(request.name, msg_buf.name);
        sprintf(request.text, "new client: %s", msg_buf.name);
        printf("ThreadReceiveClient IMPORTANT check: text = %s len = %d status = %d\n", storage->msg[storage->len].text, storage->len, storage->msg[storage->len].status);
        AddStorageMessege(storage, &request);
        AddNameList(list, msg_buf.name);
        AddNameList(&list_copy, msg_buf.name);
      }
    }
    usleep(10000);
  }
  mq_close(ds_queue_server);
  mq_close(ds_queue_register);
  for(int i = 0; i < list_copy.len; i++) mq_unlink(list_copy.name[i]);
  mq_unlink(NAME_QUEUE_REGISTER);
  mq_unlink(NAME_QUEUE_SERVER);
  printf("ThreadReceiveClient end\n");
}
