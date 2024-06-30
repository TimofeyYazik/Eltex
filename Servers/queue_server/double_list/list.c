#include <stdlib.h>
#include "list.h"

#define ERROR 1
#define OK 0

void InsertEnd(ListServer *head, ListServer *new_element) {
  ListServer *buff = head;
  while (buff->next != NULL) {
    buff = buff->next;
  }
  buff->next = new_element;
  new_element->prev = buff;
  new_element->next = NULL;
}

ListServer *SearchFD(int fd, ListServer *head) {
  ListServer *buff = head;
  while (buff != NULL) {
    if (buff->active_fd == fd)
      return buff;
    buff = buff->next;
  }
  return NULL;
}

ListServer *SearchFree(ListServer *head) {
  ListServer *buff = head;
  while (buff != NULL) {
    if (buff->busy == 0 && buff->active_fd != -1)
      return buff;
    buff = buff->next;
  }
  return NULL;
}

int Remove(ListServer *el) {
  if (el == NULL) {
    return ERROR;
  }
  if (el->prev != NULL) {
    el->prev->next = el->next;
  }
  if (el->next != NULL) {
    el->next->prev = el->prev;
  }
  free(el);
  return OK;
}

ListServer *CreateList() {
  ListServer *head = (ListServer *)malloc(sizeof(ListServer));
  if (head != NULL) {
    head->active_fd = -1;
    head->busy = 0;
    head->prev = NULL;
    head->next = NULL;
  }
  return head;
}

