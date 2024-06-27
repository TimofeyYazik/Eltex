#include <stdio.h>
#include <signal.h>


int main(){
  sigset_t set;
  int sig = 0;
  sigemptyset(&set);
  sigaddset(&set, SIGUSR1);
  sigprocmask(SIG_BLOCK, &set, NULL);
  sigwait(&set, &sig);
  printf("WAIT SUCCESS\n");
  return 0;
}







