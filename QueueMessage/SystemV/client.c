#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>


char buf[256] = "/Users/user/Eltex/QueueMessage/SistemV";

typedef struct{
  long mtype;       /* message type, must be > 0 */
  char mtext[12];    /* message data */
}msgbuf;

int main(){
  msgbuf message = {0};

  key_t key = ftok(buf, 0);
  int ds_queue = msgget(key, 0666 | IPC_CREAT);
  msgrcv(ds_queue, &message, sizeof(message), 1, 0);
  printf("%s\n", message.mtext);
  strcpy(message.mtext, "hi   ");
  message.mtype = 2;
  msgsnd(ds_queue, &message, sizeof(message), 0);
  exit(EXIT_SUCCESS);
}