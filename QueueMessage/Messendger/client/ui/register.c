#include "ui.h"
#include "../thread/thread.h"

void SigWinch(int signo)
{
  struct winsize size;
  ioctl(fileno(stdout), TIOCGWINSZ, (char *) &size); 
  resizeterm(size.ws_row, size.ws_col);
}

void Register(ControllerClient *cont) {
  cont->name[0] = '/';
  mode_t mode_mqueue = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
  char name_is_register = 0;
  Message request = {0};
  Message response = {0};
  request.status = IS_REG;
  struct mq_attr attr;
  InitAttr(&attr, sizeof(Message));
  mqd_t ds_queue_register = mq_open(NAME_QUEUE_REGISTER, O_CREAT | O_RDONLY, mode_mqueue, &attr);
  mqd_t ds_queue_server = mq_open(NAME_QUEUE_SERVER, O_CREAT | O_WRONLY, mode_mqueue, &attr);
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
    wgetnstr(wnd, cont->name + 1, MAX_NAME_LEN - 2); 
    strcpy(request.name, cont->name);
    if (mq_send(ds_queue_server, (char *)&request, MAX_NAME_LEN, 0) == -1) {
      perror("mq_send");
      exit(EXIT_FAILURE);
    }
    if (mq_receive(ds_queue_register, (char *)&response, MAX_NAME_LEN, NULL) == -1) {
      perror("mq_receive");
      exit(EXIT_FAILURE);
    }
    if (response.status == GOOD_STATUS) break;
    if (response.status == BAD_STATUS) name_is_register = 1;
  }
  mq_close(ds_queue_register);
  mq_close(ds_queue_server);
  wrefresh(wnd);
  delwin(wnd);
  endwin();
  }
