#include <stdio.h>
#include <signal.h>

int main(){
  sigset_t set;
  sigemptyset(&set);
  sigaddset(&set, SIGINT);
  sigprocmask(SIG_BLOCK, &set, NULL);
  while(1){
    int k = 1;
    scanf("%d", &k);
    if(!k) break;
  }
  return 0;
}
