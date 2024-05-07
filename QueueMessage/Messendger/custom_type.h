#ifndef CUSTOM_TYPE_H_
#define CUSTOM_TYPE_H_

#include <mqueue.h>

#define MAX_NAME_LEN 16
#define MAX_TEXT_LEN 128
#define MAX_PATH_LENGTH 256
#define  NAME_QUEUE_REGISTER "/register"
#define  NAME_QUEUE_SERVER "/server"
#define  NAME_QUEUE_NAME "/name"
#define  GOOD_STATUS "OK"
#define  BAD_STATUS "NO"

typedef struct{
  char name[MAX_NAME_LEN];
  char text[MAX_TEXT_LEN];
} Message;

typedef struct{
  Message *msg;
  int size;
  int len;
} MessageStorage;

typedef struct{
  char **name;
  int len;
  int size;
} NameList;

typedef struct{
  mqd_t *ds;
  int len;
  int size;
} DsList;

typedef struct{
  int stop_server;
  NameList *list;
  MessageStorage *storage;
  DsList *ds_list;
} Controller;

#endif // CUSTOM_TYPE_H_