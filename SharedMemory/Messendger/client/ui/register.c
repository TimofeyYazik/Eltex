#include <curses.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pthread.h>

#include "ui.h"

extern char name[MAX_NAME_LEN];

void SigWinch(int signo)
{
  struct winsize size;
  ioctl(fileno(stdout), TIOCGWINSZ, (char *) &size); 
  resizeterm(size.ws_row, size.ws_col);
}

void Register(){
  char name_is_register = 0;
  char request[MAX_NAME_LEN] = {0};
  WINDOW * wnd;
  initscr();
  signal(SIGWINCH, SigWinch); 
  curs_set(TRUE);
  int x, y;
  clear();
    getmaxyx(stdscr, y, x);
    wnd = newwin(y / 2, x / 2, (y - y / 2) / 2, (x - x / 2) / 2);
    box(wnd, 0, 0);
    wmove(wnd, 2, 4);
    if (name_is_register == 0)
      wprintw(wnd,"Enter your name: "); 
    else
      wprintw(wnd,"Enter your name again,  previous name taken: ");
    wrefresh(wnd);
    name[0] = '/';
    wgetnstr(wnd, name + 1, MAX_NAME_LEN - 2); 
  wrefresh(wnd);
  refresh();
  delwin(wnd);
  endwin();
  }
