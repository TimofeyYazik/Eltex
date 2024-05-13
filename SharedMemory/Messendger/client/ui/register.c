// #include "../thread/thread.h"
#include <semaphore.h>

#include "ui.h"
#include "../../lib/lib_mess.h"

void Register(Controller *cont) {
  WINDOW * wnd;
  initscr();
  curs_set(TRUE);
  int x, y;
    clear();
    getmaxyx(stdscr, y, x);
    wnd = newwin(y / 5, x / 2, (y / 5) * 2, (x / 4));
    box(wnd, 0, 0);
    wmove(wnd, 2, 4);
    wprintw(wnd,"Enter your name: "); 
    wrefresh(wnd);
    sem_wait(cont->sem);
    wgetnstr(wnd, cont->name_user, MAX_NAME_LEN - 1); 
    AddNameList(&cont->list, (char*)cont->name_user);
    Message msg = {0};
    strcpy(msg.name, "server");
    sprintf(msg.text, "new client: %s", cont->name_user);
    AddStorageMessege(&cont->storage, &msg);
    sem_post(cont->sem);
  wrefresh(wnd);
  refresh();
  delwin(wnd);
  endwin();
  }
