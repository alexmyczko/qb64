#ifndef INC_MEMBLOCKS_H
#define INC_MEMBLOCKS_H

#include "common.h"

extern uint64 mem_lock_id;
extern int32 mem_lock_max;
extern int32 mem_lock_next;
extern mem_lock *mem_lock_base;
extern mem_lock *mem_lock_tmp;

extern int32 mem_lock_freed_max;
extern int32 mem_lock_freed_n;
extern ptrszint *mem_lock_freed;

void new_mem_lock();
void free_mem_lock(mem_lock *lock);

#endif
