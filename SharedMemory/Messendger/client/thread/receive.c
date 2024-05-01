#include "thread.h"

extern char name[MAX_NAME_LEN];
extern int stop_client;

void *ThreadReceiveServer(void *arg){
  sem_t *sem = (sem_t *)arg;
  // int storage_len = 0;
  // int x, y;
  // getmaxyx(stdscr, y, x);
  // WINDOW *wnd = newwin((y / 4) * 3, (x / 4) * 3, 0, 0);
  // box(wnd, 0, 0);
  int fd = shm_open(NAME_SHARE_MEMORY, O_CREAT | O_RDWR, S_IWUSR | S_IRUSR);
  ftruncate(fd, SIZE_MEMORY);
  if(fd == -1) {
    perror("shm_open");
    exit(1);
  }
  SharedStructMemory *handler_ptr = (SharedStructMemory*)mmap(NULL, SIZE_MEMORY, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  printf("%s %s\n", handler_ptr->list->name[0], handler_ptr->list->name[1]);
  // while (stop_client) {
  //   if(storage_len != handler_ptr->storage.len) {
  //     storage_len = handler_ptr->storage.len;
  //     MessageWindow(wnd, &handler_ptr->storage);
  //   }
  //   sleep(1);
  // }
  // delwin(wnd);
  return NULL;
}