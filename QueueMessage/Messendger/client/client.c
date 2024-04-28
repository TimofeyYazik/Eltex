#include <curses.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <mqueue.h>
#include <unistd.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>

#include "ui/ui.h"
#include "thread/thread.h"

char name[MAX_NAME_LEN] = {0};
MessageStorage storage;
int stop_client = 1;

int main(){
  pthread_t thread_send;
  pthread_t thread_receive;
  pthread_t thread_user;
  Register();
  initscr();
  pthread_create(&thread_send, NULL, ThreadSendServer, NULL);
  pthread_create(&thread_user, NULL, ThreadUserWindow, NULL);
  pthread_create(&thread_receive, NULL, ThreadReceiveServer, NULL);
  while (stop_client);
  endwin();
  
  pthread_join(thread_send, NULL);
  pthread_join(thread_receive, NULL);
  pthread_join(thread_user, NULL);
  free(storage.msg);
  exit(EXIT_SUCCESS);
}
