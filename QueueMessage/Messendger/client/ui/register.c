#include "ui.h"
#include "../thread/thread.h"

void SigWinch(int signo)
{
  struct winsize size;
  ioctl(fileno(stdout), TIOCGWINSZ, (char *) &size); 
  resizeterm(size.ws_row, size.ws_col);
}

void Register(ControllerClient *cont) {
  mode_t mode_mqueue = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
  char name_is_register = 0;
  char request[MAX_NAME_LEN] = {0};
  struct mq_attr attr;
  InitAttr(&attr, MAX_NAME_LEN);
  mqd_t ds_queue = mq_open(NAME_QUEUE_REGISTER, O_CREAT | O_RDWR, mode_mqueue, &attr);
  if (ds_queue == -1) {
    fprintf(stderr, "Register mq_open failed with error: %d\n", errno);
    perror("mq_open");
    return;
  }
  WINDOW * wnd;
  initscr();
  signal(SIGWINCH, SigWinch); 
  curs_set(TRUE);
  int x, y;
  while (1) {
    clear();
    getmaxyx(stdscr, y, x);
    wnd = newwin(y / 5, x / 2, (y / 5) * 2, (x / 4));
    box(wnd, 0, 0);
    wmove(wnd, 2, 4);
    if   (name_is_register == 0) wprintw(wnd,"Enter your name: "); 
    else wprintw(wnd,"Enter your name again, previous name taken: ");
    wrefresh(wnd);
    cont->name[0] = '/';
    wgetnstr(wnd, cont->name + 1, MAX_NAME_LEN - 2); 
    if (mq_send(ds_queue, cont->name, MAX_NAME_LEN, 0) == -1) {
      perror("mq_send");
      exit(EXIT_FAILURE);
    }
    usleep(100000);
    if (mq_receive(ds_queue, request, MAX_NAME_LEN, NULL) == -1) {
      perror("mq_receive");
      exit(EXIT_FAILURE);
    }
    if (strcmp(request, "OK") == 0) break;
    if (strcmp(request, "NO") == 0) name_is_register = 1;
  }
  mq_close(ds_queue);
  wrefresh(wnd);
  refresh();
  delwin(wnd);
  endwin();
  }
