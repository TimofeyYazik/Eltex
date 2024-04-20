#include <pthread.h>
#include <stdio.h>

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

void *tread_calc(void *argc){
  int *i = (int *) argc;
  for (int j = 0; j < 1000000; j++){
    pthread_mutex_lock(&m);
    *i += 1;
    pthread_mutex_unlock(&m);
  }
  return NULL;
}

int main(){
  pthread_t thread[10];
  int *s;
  int battery = 0;
  for(int i = 0; i < 10; i++){
    pthread_create(&thread[i], NULL, tread_calc, &battery);
  }
  for(int i = 0; i < 10; i++){
    pthread_join(thread[i], (void **)&s);
  }
  printf("%d\n", battery);
}

