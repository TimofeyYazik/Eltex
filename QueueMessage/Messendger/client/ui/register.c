#include <curses.h>
#include <mqueue.h>
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
  char request[3] = {0};
  struct mq_attr attr;
  attr.mq_flags = 0;
  attr.mq_maxmsg = 50;
  attr.mq_msgsize = MAX_NAME_LEN;
  attr.mq_curmsgs = 0;
  mqd_t ds_queue = mq_open(NAME_QUEUE_REGISTER, O_CREAT | O_RDWR, S_IWUSR | S_IRUSR, &attr);
  WINDOW * wnd;
  initscr();
  signal(SIGWINCH, SigWinch); 
  curs_set(TRUE);
  int x, y;
  while (1) {
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
    if (mq_send(ds_queue, name, MAX_NAME_LEN, 0) == -1)
    {
      perror("mq_send");
      exit(EXIT_FAILURE);
    }
    sleep(5);
    if (mq_receive(ds_queue, request, strlen("OK") + 1, NULL) == -1)
    {
      perror("mq_receive");
      exit(EXIT_FAILURE);
    }
    if (strcmp(name, "OK") == 0)
      break;
    if (strcmp(name, "NO") == 0){
       name_is_register = 1;
    }
  }
  mq_close(ds_queue);
  wrefresh(wnd);
  refresh();
  delwin(wnd);
  endwin();
  }
