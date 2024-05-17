#include "../thread/thread.h"
#include <semaphore.h>

#include "ui.h"
#include "../../lib/lib_mess.h"

extern pthread_mutex_t mutex;
extern char name_user[MAX_NAME_LEN];

void Register(Controller *ctl) {
  WINDOW * wnd;
  mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
  sem_t *sem = sem_open(NAME_SEMAPHORE, O_RDWR, mode);
  NameList *list = &ctl->list;
  initscr();
  curs_set(TRUE);
  int x, y;
  clear();
  getmaxyx(stdscr, y, x);
  wnd = newwin(y / 5, x / 2, (y / 5) * 2, (x / 4));
  box(wnd, 0, 0);
  char is_name_exist = 1;
  while (1) {
    if(is_name_exist) mvwprintw(wnd, 2, 4, "Enter your name: "); 
    else mvwprintw(wnd, 2, 4, "Enter your name, previously used: "); 
    wrefresh(wnd);
    sem_wait(sem);
    wgetnstr(wnd, name_user, MAX_NAME_LEN - 1); 
    for (int i = 0; i < list->len; i++) {
      if (strcmp(name_user, list->name[i]) == 0) {
        is_name_exist = 0;
        break;
      } else is_name_exist = 1;
    }
    if(!is_name_exist) {
      sem_post(sem);
      continue;
    }
    strcpy(list->name[list->len], name_user);
    list->len++;
    Message msg = {0};
    strcpy(msg.name, "server");
    sprintf(msg.text, "new client: %s", list->name[list->len - 1]);
    AddStorageMessege(&ctl->storage, &msg);
    sem_post(sem);
    wrefresh(wnd);
    break;
  }
    
  delwin(wnd);
  sem_close(sem);
  endwin();
}
