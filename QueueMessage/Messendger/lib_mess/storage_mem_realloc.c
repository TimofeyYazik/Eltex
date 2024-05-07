#include "lib_mess.h"

void StorageMemRealloc(MessageStorage *storage){
  storage->size = storage->size * 2 - (storage->size / 2);
  storage->msg = realloc(storage->msg, sizeof(Message) * storage->size);
}