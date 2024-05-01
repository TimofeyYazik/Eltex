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
  sem_t *sem = (sem_t *)arg;
  getmaxyx(stdscr, y, x);
  WINDOW *wnd = newwin(y / 4, x, (y / 4) * 3, 0);
  Message msg = {0};
  strcpy(msg.name, name);
  int fd = shm_open(NAME_SHARE_MEMORY, O_CREAT | O_RDWR, S_IWUSR | S_IRUSR);
  if(fd == -1) {
    perror("shm_open");
    exit(1);
  }
  ftruncate(fd, SIZE_MEMORY);
  SharedStructMemory *handler_ptr = (SharedStructMemory*)mmap(NULL, SIZE_MEMORY, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  while (stop_client) {
    InputMessageWindow(wnd, &msg);
    if (!strcmp(msg.text, "/exit")) {
      stop_client = 0;  
      break;
    }
    sem_wait(sem);
    MsgCopy(&handler_ptr->storage.msg[handler_ptr->storage.len], &msg);
    handler_ptr->storage.len++;
    if (handler_ptr->storage.len == handler_ptr->storage.size) {
      handler_ptr->storage.size = 2 * handler_ptr->storage.size - (handler_ptr->storage.size / 2);
      handler_ptr->storage.msg = realloc(handler_ptr->storage.msg, sizeof(Message) * handler_ptr->storage.size);
    }
    sem_post(sem);
    sleep(1);
  }
  delwin(wnd);
  return NULL;
}