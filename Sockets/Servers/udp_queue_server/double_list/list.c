#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "list.h"

#define ERROR 1
#define OK 0


void FreeList(ListServer *head){
  ListServer *buff = head;
  while(buff->next){
    buff = buff->next;
    free(buff->prev);
  }
  free(buff);
}

void InsertEnd(ListServer *head, ListServer *new_element) {
  ListServer *buff = head;
  while (buff->next != NULL) {
    buff = buff->next;
  }
  buff->next = new_element;
  new_element->prev = buff;
  new_element->next = NULL;
}

ListServer *SearchFree(ListServer *head) {
  ListServer *buff = head;
  while (buff != NULL) {
      if (buff->busy == 0)
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
    memset(&head->sock, 0, sizeof(head->sock));
    head->busy = -1;
    head->prev = NULL;
    head->next = NULL;
  }
  return head;
}

