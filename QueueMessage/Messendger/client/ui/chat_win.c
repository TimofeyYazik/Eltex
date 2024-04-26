#include <curses.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>

#include "ui.h"

extern name[MAX_NAME_LEN + 1];
  
#define MAX_PATH_LENGTH 256

void MessageWindow(WINDOW *wnd, Message **msg){
  signal(SIGWINCH, SigWinch); 
  curs_set(TRUE);
  wclear(wnd);
  box(wnd, 0, 0);
  wrefresh(wnd);
  wmove(wnd, 2, 4);
  wprintw(wnd,"Enter your name: ");
  wrefresh(wnd);
  delwin(wnd);
}

void UserWindow(WINDOW *wnd, Message **msg){
  signal(SIGWINCH, SigWinch); 
  curs_set(TRUE);
  wclear(wnd);
  box(wnd, 0, 0);
  wrefresh(wnd);
}

void InputMessageWindow(WINDOW *wnd, Message *msg){
  signal(SIGWINCH, SigWinch); 
  curs_set(TRUE);
  wclear(wnd);
  wrefresh(wnd);
  wmove(wnd, 2, 4);
  wprintw(wnd,"Enter your message: ");
  wgetnstr(wnd, msg->text, MAX_TEXT_LEN);
  wrefresh(wnd);
  exit(EXIT_SUCCESS);
}

