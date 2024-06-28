#ifndef _LIST_H_
#define _LIST_H_


typedef struct list_t {
  int active_fd;
  int ip_adress;
  struct list_t *prev;
  struct list_t *next;
} ListServer;

void InsertEnd(ListServer *head, ListServer *new_element);
ListServer *SearchFD(int fd, ListServer *head); 
ListServer *SearchIP(int ip, ListServer *head); 
int Remove(ListServer *el);
ListServer *CreateList();

#endif // _LIST_H_
