#include "thread.h"

extern MessageStorage storage;
extern volatile int stop_server;


void MsgCopy(Message *dst, Message *src){
  strcpy(dst->name, src->name);
  strcpy(dst->text, src->text);
}

void *ThreadReceiveClient(void *arg){
  fprintf(stderr, "ThreadReceiveClient start\n");
  Message msg_buf = {0};
  storage.size = 50;
  storage.msg = malloc(sizeof(Message) * storage.size);
  struct mq_attr attr;
  attr.mq_flags = 0;
  attr.mq_maxmsg = 50;
  attr.mq_msgsize = sizeof(Message);
  attr.mq_curmsgs = 0;
  mqd_t ds_queue_server = mq_open(NAME_QUEUE_SERVER, O_CREAT | O_RDONLY, S_IWUSR | S_IRUSR, &attr);
  if (ds_queue_server == -1) {
    fprintf(stderr, "ThreadReceiveClient mq_open failed with error: %d\n", errno);
    perror("mq_open");
    return NULL;
  }
  while(stop_server) {
    mq_receive(ds_queue_server, (char*)&msg_buf, sizeof(Message), NULL);
    if(strcmp(msg_buf.text, "/exit")){
      sprintf(msg_buf.text, "client is out: %s", msg_buf.name);
      sprintf(msg_buf.name, "server");
    }
    MsgCopy(&storage.msg[storage.len], &msg_buf);
    fprintf(stderr ,"ThreadReceiveClient check: %s\n", storage.msg[storage.len].text);
    storage.len++;
    if (storage.len == storage.size) {
      storage.size = storage.size * 2 - (storage.size / 2);
      storage.msg = realloc(storage.msg, sizeof(Message) * storage.size);
    }
    usleep(10000);
    memset(msg_buf.text, 0, sizeof(msg_buf.text));
    memset(msg_buf.name, 0, sizeof(msg_buf.name));
  }
  mq_close(ds_queue_server);
  mq_unlink(NAME_QUEUE_SERVER);
  printf("ThreadReceiveClient end\n");
}