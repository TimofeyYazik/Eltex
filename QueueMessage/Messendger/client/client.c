#include <curses.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/ioctl.h>
#include <mqueue.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>

#include "ui/ui.h"
#include "../custom_type.h"

char name[MAX_NAME_LEN + 1] = {0};



void *ThreadSendServer(void *arg){
  struct mq_attr attr;
  mqd_t ds_queue_server = mq_open(NAME_QUEUE_SERVER, O_CREAT | O_WRONLY, S_IWUSR | S_IRUSR, &attr);
  Message msg = {0};
  attr.mq_flags = 0;
  attr.mq_maxmsg = 50;
  attr.mq_msgsize = sizeof(msg);
  attr.mq_curmsgs = 0;
  int x, y;
  getmaxyx(stdscr, y, x);
  strcpy(msg.name, name);
  WINDOW *wnd = newwin(y / 4, x, (y / 4) * 3, 0);
  box(wnd, 0, 0);
  while (1) {
    InputMessageWindow(wnd, &msg);
    printf("%d\n", sizeof(msg));
    if(mq_send(ds_queue_server, (char*)&msg, sizeof(msg), 0) == -1){
      perror("mq_send");
    }
    
  }
  mq_close(ds_queue_server);
}

void *ThreadReceiveServer(void *arg){
  int size = 0;
  MessageStorage storage;
  storage.len = 0;
  storage.size = 50;
  storage.msg = malloc(sizeof(Message) * storage.size);
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
  WINDOW *wnd = newwin(y / 4, x, (y / 4) * 3, 0);
  box(wnd, 0, 0);
  while (1) {
    MessageWindow(wnd, &storage);
    mq_receive(ds_queue_connect, (char*)&msg, sizeof(Message), NULL);
    storage.msg[storage.len] = msg;
    storage.len++;
    if (storage.len == storage.size) {
      storage.size *= 2 - (storage.size / 2);
      storage.msg = realloc(storage.msg, sizeof(Message) * storage.size);
    }
    usleep(10000);
    memset(msg.text, 0, sizeof(msg.text));
  }
  mq_close(ds_queue_connect);
  mq_unlink(name);
}

int main(){
  pthread_t thread_send;
  pthread_t thread_receive;
  Register();
  initscr();
  pthread_create(&thread_send, NULL, ThreadSendServer, NULL);
  pthread_create(&thread_receive, NULL, ThreadReceiveServer, NULL);
  sleep(1215752192);
  pthread_join(thread_send, NULL);
  pthread_join(thread_receive, NULL);
  endwin();
  exit(EXIT_SUCCESS);
}
