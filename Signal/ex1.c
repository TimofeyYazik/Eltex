#include <stdio.h>
#include <signal.h>

volatile char stop = 1; 

void sig_func(int seg){
  printf("hello signal SIGUSER1: %d\n", seg);
  stop = 0;
}

int main(int argc, char *argv[])
{
  struct  sigaction settings;
  settings.__sigaction_u.__sa_handler = sig_func;
  settings.sa_mask |= SIGUSR1;
  settings.sa_flags = 0;
  sigaction(SIGUSR1, &settings, NULL);
  while(stop){}
  return 0;
}
