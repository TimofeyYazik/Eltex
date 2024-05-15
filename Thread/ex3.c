#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h> 
#include <sys/syscall.h>

#define NUM_SHOPS 5
#define NUM_CUSTOMERS 3
#define RESTOCK_PACKAGE_SIZE 5000

typedef struct{
  int id;
  int package;
} Client;

typedef struct{
  int product;
  int is_closed;
  pthread_mutex_t mutex;
} Shop;


Client customers[NUM_CUSTOMERS];
Shop shops[NUM_SHOPS];

void *CustomersBuy(void *argc){
  Client *client = (Client *) argc;
  int prev_product = 0;
  int i = 0;
  while (client->package > 0){
   for(i = 0; i < NUM_SHOPS; i++){
     if(shops[i].is_closed || shops[i].product == 0) continue;
     pthread_mutex_lock(&shops[i].mutex);
     shops[i].is_closed = 1;
     if(client->package < shops[i].product){
        shops[i].product -= client->package;
        client->package = 0; 
      } else {  
        client->package -= shops[i].product;
        prev_product = shops[i].product;
        shops[i].product = 0;
      }
     sleep(2);
     shops[i].is_closed = 0;
     pthread_mutex_unlock(&shops[i].mutex);
     printf("Customer %d bought %d from shop %d\n", client->id, prev_product, i);
     printf("Customer %d has %d package\n", client->id, client->package);
     break;
   } 
   if(i == NUM_SHOPS) {
    printf("Customer %d is out of shops\n", client->id);
    sleep(2);
   }
  }
  printf("Customer %d is out of packages\n", client->id);
  return NULL;
}

void *Provider(void *argc){
  int i = 0;
  int prev_provided = -1;
  while (1) {
    for(i = 0; i < NUM_SHOPS; i++){
      if(shops[i].is_closed || prev_provided == i) continue;
      prev_provided = i;
      pthread_mutex_lock(&shops[i].mutex);
      shops[i].is_closed = 1;
      shops[i].product += RESTOCK_PACKAGE_SIZE;
      sleep(1);
      shops[i].is_closed = 0;
      pthread_mutex_unlock(&shops[i].mutex);
      break;
    }
    if(i == NUM_SHOPS) {printf("Provider is out of shops\n"); sleep(1);}
    else printf("Provider restocked shop %d\n", i);
  }
}

int main(){
  srand(time(NULL));   \
  pthread_t customers_pthread[3];
  pthread_t provider_pthread;
  int lower = 5000, upper = 10000;
  
  for(int i = 0; i < NUM_SHOPS; i++){
    shops[i].product = (rand() % (upper - lower + 1)) + lower;
    shops[i].is_closed = 0;
  }
  for(int i = 0; i < NUM_CUSTOMERS; i++){
    customers[i].id = i;
    customers[i].package = 100000;
  }
  for(int i = 0; i < NUM_CUSTOMERS; i++){
    pthread_create(&customers_pthread[i], NULL, CustomersBuy, &customers[i]);
  }
  
  pthread_create(&provider_pthread, NULL, Provider, NULL);
  
  for(int i = 0; i < NUM_CUSTOMERS; i++){
    pthread_join(customers_pthread[i], NULL);
  }
  pthread_cancel(provider_pthread);
  for(int i = 0; i < NUM_SHOPS; i++){
    pthread_mutex_destroy(&shops[i].mutex);
  }
  exit(EXIT_SUCCESS);
}
