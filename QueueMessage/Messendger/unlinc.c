#include <mqueue.h>

int main(){
  mq_unlink("/lp");
  mq_unlink("/kl");
  mq_unlink("/hi");
  mq_unlink("/j");
  mq_unlink("/l");
  return 0;
}