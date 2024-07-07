#ifndef _LIST_H_
#define _LIST_H_


typedef struct list_t {
  int active_fd;
  struct list_t *prev;
  struct list_t *next;
} ListServer;

void InsertEnd(ListServer *head, ListServer *new_element);
int Remove(ListServer *el);
ListServer *SearchFree(ListServer *head);
void FreeList(ListServer *head);
ListServer *CreateList();

#endif // _LIST_H_
