#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

char buf[256] = "/Users/user/Eltex/QueueMessage/SistemV";

typedef struct{
  long mtype;       /* message type, must be > 0 */
  char mtext[12];    /* message data */
}msgbuf;

int main(){
  msgbuf message = {1, "hello world"};

  key_t key = ftok(buf, 0);
  int ds_queue = msgget(key, 0666 | IPC_CREAT);
  msgsnd(ds_queue, &message, sizeof(message), 0);
  ssize_t k = 0;
  while (!k)
  {
    k = msgrcv(ds_queue, &message, sizeof(message), 2, 0);
  }
  msgctl(ds_queue, IPC_RMID, NULL);
  printf("%s\n", message.mtext);
  exit(EXIT_SUCCESS);
}