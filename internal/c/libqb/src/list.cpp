//List Interface
//Purpose: Unify and optimize the way QB64 references lists of objects (such as handles)
//Notes: Does not use index 0

#include "common.h"
#include "libqb.h"
#include "mutex.h"
#include "list.h"

list *list_new(ptrszint structure_size){
  list *L;
  L=(list*)calloc(1,sizeof(list));
  L->structure=(uint8*)malloc(sizeof(uint8*));
  L->structure_base[1]=(ptrszint)L->structure;
  L->structure_bases=1;
  L->structure_freed=(ptrszint*)malloc(sizeof(ptrszint*));
  L->index=(ptrszint*)malloc(sizeof(ptrszint*));
  L->user_structure_size=structure_size;
  L->internal_structure_size=structure_size+sizeof(ptrszint);  
  return L;
}

list *list_new_threadsafe(ptrszint structure_size){
  list *L=list_new(structure_size);
  L->lock_add=new_mutex();
  L->lock_remove=new_mutex();
  return L;
}

ptrszint list_add(list *L){  
  lock_mutex(L->lock_add);
  ptrszint i;
  if(L->structures_freed){//retrieve index from freed list if possible
    lock_mutex(L->lock_remove);
    i=L->structure_freed[L->structures_freed--];
    uint8* structure;
    structure=(uint8*)L->index[i];
    memset(structure,0,L->user_structure_size);
    *(ptrszint*)(structure+L->user_structure_size)=i;
    unlock_mutex(L->lock_remove);
  }else{
    //create new buffer?
    if ((L->structures+1)>L->structures_last){
      ptrszint new_structures_last;
      new_structures_last=(L->structures_last*2)+1;
      //note: L->structure is only modified by list_add
      L->structure=(uint8*)calloc(1,L->internal_structure_size*(new_structures_last+1));
      if (L->structure==NULL){ alert("list_add: failed to allocate new buffer, structure size:"); alert(L->internal_structure_size);}
      L->structures_last=new_structures_last;
      L->structures=0;
      L->structure_base[++L->structure_bases]=(ptrszint)L->structure;
    }
    i=++L->indexes;    
    *(ptrszint*)(L->structure+(L->internal_structure_size*(++L->structures))+L->user_structure_size)=i;      
    //allocate new index
    if (L->indexes>L->indexes_last){
      if (L->index_cleanup!=NULL) free(L->index_cleanup);
      L->index_cleanup=L->index;
      int32 new_indexes_last=(L->indexes_last*2)+1;
      ptrszint* temp=(ptrszint*)malloc(sizeof(ptrszint)*(new_indexes_last+1));
      memcpy(temp,L->index,sizeof(ptrszint)*(L->indexes_last+1));
      L->index=temp;
      L->index[i]=(ptrszint)( L->structure + (L->internal_structure_size*L->structures) );
      L->indexes_last=new_indexes_last;
    }else{
      L->index[i]=(ptrszint)( L->structure + (L->internal_structure_size*L->structures) );
    }  
  }
  unlock_mutex(L->lock_add);
  return i;
}//list_add

ptrszint list_remove(list *L,ptrszint i){//returns -1 on success, 0 on failure
  lock_mutex(L->lock_remove);
  if ((i<1)||(i>L->indexes)){
    unlock_mutex(L->lock_remove);
    return 0;
  }
  uint8* structure;
  structure=(uint8*)(L->index[i]);
  if (!*(ptrszint*)(structure+L->user_structure_size)){
    unlock_mutex(L->lock_remove);
    return 0;
  }  
  //expand buffer?
  if ((L->structures_freed+1)>L->structures_freed_last){        
    ptrszint new_structures_freed_last;
    new_structures_freed_last=(L->structures_freed_last*2)+1;
    ptrszint *temp=(ptrszint*)malloc(sizeof(ptrszint)*(new_structures_freed_last+1));
    memcpy(temp, L->structure_freed, sizeof(ptrszint)*(L->structures_freed+1));
    if (L->structure_freed_cleanup!=NULL) free(L->structure_freed_cleanup);
    L->structure_freed_cleanup=L->structure_freed;
    L->structure_freed=temp;
    L->structures_freed_last=new_structures_freed_last;
  }
  L->structure_freed[L->structures_freed+1]=i;  
  *(ptrszint*)(structure+L->user_structure_size)=0;
  L->structures_freed++;
  unlock_mutex(L->lock_remove);
  return -1;
};

void list_destroy(list *L){
  ptrszint i;
  for (i=1;i<=L->structure_bases;i++){
    free((void*)L->structure_base[i]);
  }
  free(L->structure_base);
  free(L->structure_freed);
  free(L);
}

void *list_get(list *L, ptrszint i){//Returns a pointer to an index's structure
  if ((i<1)||(i>L->indexes)){
    return NULL;
  }
  uint8* structure;
  structure=(uint8*)(L->index[i]);
  if (!*(ptrszint*)(structure+L->user_structure_size)) return NULL;
  return (void*)structure;
}

ptrszint list_get_index(list *L,void *structure){//Retrieves the index value of a structure
  ptrszint i=*(ptrszint*) ( ((uint8*)structure) + L->user_structure_size );
  return i;
}
