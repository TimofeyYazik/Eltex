#include <curses.h>
#include <stdlib.h>
#include <signal.h>
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
  int x, y;
  struct mq_attr attr;
  int x, y;
  getmaxyx(stdscr, y, x);
  WINDOW * wnd = newwin((y / 4) * 3, x, 0, 0);
  mqd_t ds_queue_server = mq_open(NAME_QUEUE_SERVER, O_CREAT | O_WRONLY, S_IWUSR | S_IRUSR, &attr);
  attr.mq_flags = 0;
  attr.mq_maxmsg = 50;
  attr.mq_msgsize = sizeof(Message);
  attr.mq_curmsgs = 0;
  getmaxyx(stdscr, y, x);
  Message msg = {0};
  strcpy(msg.name, name);
  WINDOW *wnd = newwin(y / 4, x, (y / 4) * 3, 0);
  box(wnd, 0, 0);
  while (1) {
    InputMessageWindow(wnd, &msg);
    mq_send(ds_queue_server, (char*)&msg, sizeof(Message), 0);
  }
  mq_close();
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
    mq_receive(ds_queue_server, (char*)&msg, sizeof(Message), NULL);
    storage.msg[storage.len] = msg;
    storage.len++;
    if (storage.len == storage.size) {
      storage.size *= 2 - (storage.size / 2);
      storage.msg = realloc(storage.msg, sizeof(Message) * storage.size);
    }
    MessageWindow(wnd, &msg);
    usleep(10000);
    memset(msg, 0, sizeof(Message));
  }
  mq_close(ds_queue_connect);
  mq_unlink(name);
}

int main(){
  pthread_t thread_send;
  pthread_t thread_receive;
  Register();
  initscr();
  pthread_create(&thread_send, NULL, ThreadSendServer, (void *)&ds_queue_client);
  pthread_create(&thread_receive, NULL, ThreadReceiveServer, (void *)&ds_queue_server);
  pthread_join(thread_send, NULL);
  pthread_join(thread_receive, NULL);
  endwin();
  exit(EXIT_SUCCESS);
}
  // struct mq_attr attr;
  // attr.mq_flags = 0;
  // attr.mq_maxmsg = 50;
  // attr.mq_msgsize = sizeof(Message);
  // attr.mq_curmsgs = 0;
  // mqd_t ds_queue_server = mq_open(name_queue_server, O_CREAT | O_RDWR, S_IWUSR | S_IRUSR, &attr);