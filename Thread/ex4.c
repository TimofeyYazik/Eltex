#include <string.h>
#include <stdlib.h>
#include <pthread.h>

pthread_key_t key;
pthread_once_t once = PTHREAD_ONCE_INIT;

void dest(void *ptr){
  free(ptr);
}

void init_key()
{
  pthread_key_create(&key, dest);
}


char *error(int errnum)
{
  char *buf;

  pthread_once(&once, init_key);
  buf = pthread_getspecific(key);
  if(buf == NULL){
    buf = malloc(244);
    pthread_setspecific(key, buf);
  }
  switch (errnum)
  {
  case 0:
    strcpy(buf, "error 0");
    break;
  case 1:
    strcpy(buf, "error 1");
    break;
  case 2:
    strcpy(buf, "error 2");
    break;
  default:
    break;
  }
  return buf;
}