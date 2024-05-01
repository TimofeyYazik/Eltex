// #include "thread.h"

// #include "thread.h"

// extern char name[MAX_NAME_LEN];
// extern int stop_client;

// void *ThreadUserWindow(void *arg){
//   sem_t *sem = (sem_t *)arg;
//   int x, y;
//   getmaxyx(stdscr, y, x);
//   WINDOW *wnd = newwin((y / 4) * 3, x / 4, 0, (x / 4) * 3);
//     int fd = shm_open(NAME_SHARE_MEMORY, O_CREAT | O_RDWR, S_IWUSR | S_IRUSR);
    
//   ftruncate(fd, sizeof(SharedStructMemory));
//   SharedStructMemory *handler_ptr = (SharedStructMemory*)mmap(NULL, sizeof(SharedStructMemory), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

//   int flag = 1;
//   while(stop_client){
//   if(storage.len != storage_len){
//     for(int i = storage_len; i < storage.len; i++){
//       for(int j = 0; j < list.len; j++){
//         flag = strcmp(list.name[j], storage.msg[i].name);
//         if(!flag) break;
//       }
//       if(!flag) break;
//       list.name[list.len] = malloc(sizeof(char) * MAX_NAME_LEN);
//       strcpy(list.name[list.len], storage.msg[i].name);
//       list.len++;
//       if(list.len == list.size){
//         list.size = 2 * list.size - (list.size / 2);
//         list.name = realloc(list.name, sizeof(char*) * list.size);
//       }
//     }
//       UserWindow(wnd, &list);
//       storage_len = storage.len;
//     }
//     flag = 1;
//     sleep(1);
//   }
//   delwin(wnd);
// }