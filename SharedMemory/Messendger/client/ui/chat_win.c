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
  

void MessageWindow(WINDOW *wnd, MessageStorage *msg){
  curs_set(FALSE);
  wclear(wnd);
  box(wnd, 0, 0);
  wrefresh(wnd);
  for (int i = 0; i < msg->len; i++) {
    mvwprintw(wnd, i + 2, 4, "%s: %s\n", msg->msg[i].name, msg->msg[i].text);
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
  curs_set(TRUE);
  wclear(wnd);
  wrefresh(wnd);
  box(wnd, 0, 0);
  wmove(wnd, 2, 4);
  wprintw(wnd,"Enter your message: ");
  wrefresh(wnd);
  wgetnstr(wnd, msg->text, MAX_TEXT_LEN - 1);
}

