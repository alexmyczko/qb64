#ifndef INC_LIST_H
#define INC_LIST_H

#include "common.h"
#include "mutex.h"

struct list {
  ptrszint user_structure_size;
  ptrszint internal_structure_size;
  uint8 *structure;//block of structures of user-specified size
  ptrszint structures;
  ptrszint structures_last;
  ptrszint *structure_freed;//quickly re-reference available structures after they have been removed
  ptrszint *structure_freed_cleanup;//the previous *structure_freed memory block
  ptrszint structures_freed;
  ptrszint structures_freed_last;
  ptrszint structure_base[64];//every time the 'structure' block is full a new and larger block is allocated
  //because the list doubles each time, 64 entries will never be exceeded
  ptrszint structure_bases;
  ptrszint *index;//pointers to the structures referred to by each index value
  ptrszint *index_cleanup;
  ptrszint indexes;
  ptrszint indexes_last;
  MUTEX *lock_add;
  MUTEX *lock_remove;
};

list *list_new(ptrszint structure_size);
list *list_new_threadsafe(ptrszint structure_size);
ptrszint list_add(list *L);
ptrszint list_remove(list *L,ptrszint i);
void list_destroy(list *L);
void *list_get(list *L, ptrszint i);
ptrszint list_get_index(list *L,void *structure);

#endif
