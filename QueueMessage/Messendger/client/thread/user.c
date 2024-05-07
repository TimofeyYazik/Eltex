#include "thread.h"

extern char name[MAX_NAME_LEN];

void *ThreadUserWindow(void *arg){
  int x, y;
  getmaxyx(stdscr, y, x);
  WINDOW *wnd = newwin((y / 4) * 3, x / 4, 0, (x / 4) * 3);
  Controller *cont = (Controller*)arg;
  NameList *list = cont->list;
  MessageStorage *storage = cont->storage;
  int storage_len = 0;
  while(cont->stop_server){
  if(storage->len != storage_len){
    for(int i = storage_len; i < storage->len; i++){
      if(strcmp(storage->msg[i].name, "/server")) continue;
      if(strstr(storage->msg[i].text, "new client:")){
        list.name[list.len] = malloc(sizeof(char) * MAX_NAME_LEN);
        strcpy(list.name[list.len], storage->msg[i].text + 12);
      }
      if(strstr(storage->msg[i].text, "client is out: ")){
        for (int j = 0; j < list->len; j++) {
          if(strcmp(list->name[j], storage->msg[i].text + 15) == 0) {
            ShiftList(list, j);
            break;
          }
        }
      }
      list.len++;
      if(list.len == list.size){
        list.size = 2 * list.size - (list.size / 2);
        list.name = realloc(list.name, sizeof(char*) * list.size);
      }
    }
    UserWindow(wnd, &list);
    storage_len = storage->len;
  }
    sleep(1);
  }
  delwin(wnd);
}