#include <stdlib.h>
#include "common.h"
#include "memblocks.h"

//QB64 memory blocks
uint64 mem_lock_id=1073741823;//this value should never be 0 or 1
int32 mem_lock_max=10000;
int32 mem_lock_next=0;
mem_lock *mem_lock_base=(mem_lock*)malloc(sizeof(mem_lock)*mem_lock_max);
mem_lock *mem_lock_tmp;

int32 mem_lock_freed_max=1000;//number of allocated entries
int32 mem_lock_freed_n=0;//number of entries
ptrszint *mem_lock_freed=(ptrszint*)malloc(sizeof(ptrszint)*mem_lock_freed_max);

void new_mem_lock(){
  if (mem_lock_freed_n){
    mem_lock_tmp=(mem_lock*)mem_lock_freed[--mem_lock_freed_n];
  }else{
    if (mem_lock_next==mem_lock_max){mem_lock_base=(mem_lock*)malloc(sizeof(mem_lock)*mem_lock_max); mem_lock_next=0;}
    mem_lock_tmp=&mem_lock_base[mem_lock_next++];
  }
  mem_lock_tmp->id=++mem_lock_id;
}

void free_mem_lock(mem_lock *lock){
  lock->id=0;//invalidate lock
  if (lock->type==1) free(lock->offset);//malloc type
  //add to freed list
  if (mem_lock_freed_n==mem_lock_freed_max){mem_lock_freed_max*=2; mem_lock_freed=(ptrszint*)realloc(mem_lock_freed,sizeof(ptrszint)*mem_lock_freed_max);}
  mem_lock_freed[mem_lock_freed_n++]=(ptrszint)lock;
}


