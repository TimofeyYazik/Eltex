#ifndef CUSTOM_TYPE_H_
#define CUSTOM_TYPE_H_

#include <mqueue.h>

#define MAX_NAME_LEN 16
#define MAX_TEXT_LEN 128
#define MAX_PATH_LENGTH 256
#define  NAME_QUEUE_REGISTER "/register"
#define  NAME_QUEUE_SERVER "/server"
#define  NAME_QUEUE_NAME "/name"

#define  BAD_STATUS 6
#define GOOD_STATUS 7

#define IS_SHOTDOWN -1
#define IS_REG 2
#define IS_OUT 0
#define IS_ONLINE 1

typedef struct{
  int status;
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

typedef struct{
  int stop_client;
  char name[MAX_NAME_LEN];
  NameList *list;
  MessageStorage *storage;
} ControllerClient;

#endif // CUSTOM_TYPE_H_