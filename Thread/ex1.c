#include <pthread.h>
#include <stdio.h>

void *tread_print(void *argc){
  int *i = (int *) argc;
  printf("thread num %d\n", *i);
  return NULL;
}


int main(){
  pthread_t thread[10];
  int *s;
  int battery[10] = {0};
  for(int i = 0; i < 10; i++){
    battery[i] = i;
    pthread_create(&thread[i], NULL, tread_print, &battery[i]);
  }
  for(int i = 0; i < 10; i++){
    pthread_join(thread[i], (void **)&s);
  }
}

