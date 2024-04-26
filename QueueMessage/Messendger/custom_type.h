#ifndef CUSTOM_TYPE_H_
#define CUSTOM_TYPE_H_

#include <mqueue.h>

#define MAX_NAME_LEN 16
#define MAX_TEXT_LEN 128
#define MAX_PATH_LENGTH 256
#define  NAME_QUEUE_REGISTER "/register"
#define  NAME_QUEUE_SERVER "/server"
#define  GOOD_STATUS "OK"
#define  BAD_STATUS "NO"

typedef struct{
  int id;
  char name[MAX_NAME_LEN];
  char type;
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

#endif // CUSTOM_TYPE_H_