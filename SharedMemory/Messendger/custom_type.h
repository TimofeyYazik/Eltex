#ifndef CUSTOM_TYPE_H_
#define CUSTOM_TYPE_H_


#define MAX_NAME_LEN 16
#define MAX_TEXT_LEN 128
#define NAME_SEMAPHORE "/semaphore"
#define NAME_SHARE_MEMORY "/share_memory"
#define GOOD_STATUS "OK"
#define BAD_STATUS "NO"
#define SIZE_MEMORY 8256

typedef struct{
  char *name;
  char *text;
} Message;

typedef struct{
  Message *msg;
  int size;
  int len;
} MessageStorage;

// typedef struct{
//   char **name;
//   int len;
//   int size;
// } NameList;

// typedef struct{
//   NameList *list;
//   MessageStorage *storage;
// } SharedStructMemory;


#endif // CUSTOM_TYPE_H_