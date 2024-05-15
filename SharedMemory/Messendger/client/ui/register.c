// #include "../thread/thread.h"
#include <semaphore.h>

#include "ui.h"
#include "../../lib/lib_mess.h"

extern pthread_mutex_t mutex;

void Register(Controller *cont, char *name_user) {
  WINDOW * wnd;
  NameList *list = &cont->list;
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
  // sem_wait(cont->sem);
  wgetnstr(wnd, list->name[list->len], MAX_NAME_LEN - 1); 
  strcpy(name_user, list->name[list->len]);
  list->len++;
  Message msg = {0};
  strcpy(msg.name, "server");
  sprintf(msg.text, "new client: %s", list->name[list->len - 1]);
  AddStorageMessege(&cont->storage, &msg);
  // sem_post(cont->sem);
  wrefresh(wnd);
  refresh();
  delwin(wnd);
  endwin();
  }
