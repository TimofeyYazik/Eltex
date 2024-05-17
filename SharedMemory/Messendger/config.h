#ifndef CUSTOM_TYPE_H_
#define CUSTOM_TYPE_H_

#include <semaphore.h>

#define MAX_NAME_LEN 16
#define MAX_TEXT_LEN 128
#define NAME_SEMAPHORE "/semaphore"
#define NAME_SHARE_MEMORY "/share_memory"
#define MAX_MESSAGE_SIZE 42
#define MAX_NAMELIST_SIZE 32

typedef struct{
  int status;
  char name[MAX_NAME_LEN];
  char text[MAX_TEXT_LEN];
} Message;

typedef struct{
  Message msg[MAX_MESSAGE_SIZE];
  int len;
} MessageStorage;

typedef struct{
  char name[MAX_NAMELIST_SIZE][MAX_NAME_LEN];
  int len;
} NameList;

typedef struct{
  NameList list;
  MessageStorage storage;
} Controller;


#endif // CUSTOM_TYPE_H_