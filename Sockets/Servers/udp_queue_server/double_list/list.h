#ifndef _LIST_H_
#define _LIST_H_
#include <netinet/in.h>

typedef struct list_t {
  struct sockaddr_in sock;
  int busy;
  struct list_t *prev;
  struct list_t *next;
} ListServer;

void InsertEnd(ListServer *head, ListServer *new_element);
int Remove(ListServer *el);
ListServer *SearchFree(ListServer *head);
ListServer *CreateList();

#endif // _LIST_H_