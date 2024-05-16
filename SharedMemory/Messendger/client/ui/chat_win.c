#include <curses.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>

#include "ui.h"
  
extern pthread_mutex_t mutex;

void MessageWindow(WINDOW *wnd, MessageStorage *msg , int y){
  curs_set(FALSE);
  int y1 = y - 3;
  wclear(wnd);
  box(wnd, 0, 0);
  wrefresh(wnd);
  int y_shift = 0;
  if(msg->len > y1){
    y_shift = msg->len - y1;
    y1 = (msg->len - 3);
  }
  for (int i = y_shift, j = 0; i < msg->len; i++, j++) {
    mvwprintw(wnd, j + 2, 4, "%s: %s\n", msg->msg[i].name, msg->msg[i].text);
  }
  wrefresh(wnd);
}

void UserWindow(WINDOW *wnd, NameList *list){
  curs_set(FALSE);
  wclear(wnd);
  box(wnd, 0, 0);
  for(int i = 0; i < list->len; i++){
    if(strcmp(list->name[i], "/server") == 0) continue;
    mvwprintw(wnd, i + 2, 4, "%s\n", list->name[i]);
  }
  wrefresh(wnd);
}

void InputMessageWindow(WINDOW *wnd, Message *msg){
  pthread_mutex_lock(&mutex);
  curs_set(TRUE);
  wclear(wnd);
  wrefresh(wnd);
  box(wnd, 0, 0);
  wmove(wnd, 2, 4);
  wprintw(wnd,"Enter your message: ");
  wrefresh(wnd);
  pthread_mutex_unlock(&mutex);
  wgetnstr(wnd, msg->text, MAX_TEXT_LEN - 1);
}

