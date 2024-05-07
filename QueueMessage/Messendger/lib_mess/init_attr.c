#include "lib_mess.h"

void InitAttr(struct mq_attr *attr, size_t size){
  attr->mq_flags = 0;
  attr->mq_maxmsg = 50;
  attr->mq_msgsize = size;
  attr->mq_curmsgs = 0;
}