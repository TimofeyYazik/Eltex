#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> 
#include <sys/syscall.h>

pthread_t id_provider;
char shop_close[5] = {0};
int shops[5] = {0};
pthread_mutex_t mutex_shops = PTHREAD_MUTEX_INITIALIZER; 

void *customers_buy(void *argc){
  int id = pthread_self();
  int *customers = (int *)argc;
  
  while(*customers != 0){
    int i = 0;  
    printf("id thread = %ld\n", id);
    printf("need %d\n", *customers);
    pthread_mutex_lock(&mutex_shops);
    int shop_index = -1;
    for (; i < 5; i++) {
      if (shop_close[i] == 0) {
        shop_close[i] = 1; // Mark the shop as closed
        shop_index = i;
        break;
      }
    }
    pthread_mutex_unlock(&mutex_shops);
    if (shop_index == -1) {
      printf("All shops are busy, waiting...\n");
      sleep(2);
      continue;
    }
    if (*customers >= shops[shop_index]) {
      *customers -= shops[shop_index];
      shops[shop_index] = 0;
    } else {
      shops[shop_index] -= *customers;
      *customers = 0;
    }
    
    shop_close[shop_index] = 0;
    sleep(2);
  }
  
  return NULL; 
}

void *provider(void *argc){
  int package = 5000;
  id_provider = pthread_self();
  
  while(1){
    printf("id provider thread = %ld\n", id_provider);
    printf("+5000\n");
    
    pthread_mutex_lock(&mutex_shops);
    int shop_index = -1;
    for (int i = 0; i < 5; i++) {
      if (shop_close[i] == 0) {
        shop_index = i;
        shop_close[i] = 1;
        break;
      }
    }
    pthread_mutex_unlock(&mutex_shops);
    if (shop_index == -1) {
      printf("All shops are busy for restocking, waiting...\n");
      sleep(1);
      continue;
    }
    
    shops[shop_index] += package;
    shop_close[shop_index] = 0;
    sleep(1);
  }
  
  return NULL;
}

int main(){
  srand(time(NULL));   // Initialization, should only be called once.
  pthread_t customers_pthread[3];
  pthread_t provider_pthread;
  int lower = 5000, upper = 10000;
  
  for(int i = 0; i < 5; i++){
    shops[i] = (rand() % (upper - lower + 1)) + lower;
  }
  
  int customers_arr[3] = {100000}; 
  
  for(int i = 0; i < 3; i++){
    pthread_create(&customers_pthread[i], NULL, customers_buy, &customers_arr[i]);
  }
  
  pthread_create(&provider_pthread, NULL, provider, NULL);
  
  for(int i = 0; i < 3; i++){
    pthread_join(customers_pthread[i], NULL);
  }
  
  pthread_cancel(id_provider);
  exit(EXIT_SUCCESS);
}