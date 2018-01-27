#ifndef INCLUDE_QBS_H
#define INCLUDE_QBS_H

#include "common.h"

extern qbs *qbs_malloc;
extern uint32 qbs_malloc_next;
extern ptrszint *qbs_malloc_freed;
extern uint32 qbs_malloc_freed_size;
extern uint32 qbs_malloc_freed_num;

//Used to track strings in 16bit memory
extern ptrszint *qbs_cmem_list;
extern ptrszint *qbs_cmem_list;
extern uint32  qbs_cmem_list_lasti;
extern uint32  qbs_cmem_list_nexti;
//Used to track strings in 32bit memory
extern ptrszint *qbs_list;
extern uint32  qbs_list_lasti;
extern uint32  qbs_list_nexti;
//Used to track temporary strings for later removal when they fall out of scope
//*Some string functions delete a temporary string automatically after they have been
// passed one to save memory. In this case qbstring_templist[?]=0xFFFFFFFF
extern ptrszint *qbs_tmp_list;
extern uint32  qbs_tmp_list_lasti;
extern uint32 qbs_tmp_list_nexti; //Not defined in qbs.cpp, need to find a better spot for this
//entended string memory
extern uint8 *qbs_data;
extern uint32 qbs_data_size;
extern uint32 qbs_sp;

qbs *qbs_new_descriptor();
void qbs_free_descriptor(qbs *str);
void qbs_free(qbs *str);
void qbs_cmem_concat_list();
void qbs_concat_list();
void qbs_tmp_concat_list();
void qbs_concat(uint32 bytesrequired);
void qbs_concat_cmem(uint32 bytesrequired);
qbs *qbs_new_cmem(int32 size,uint8 tmp);
qbs *qbs_new_txt(const char *txt);
qbs *qbs_new_txt_len(const char *txt,int32 len);
qbs *qbs_new_fixed(uint8 *offset,uint32 size,uint8 tmp);
qbs *qbs_new(int32 size,uint8 tmp);
void qbs_maketmp(qbs *str);
qbs *qbs_set(qbs *deststr,qbs *srcstr);
qbs *qbs_add(qbs *str1,qbs *str2);
qbs *qbs_ucase(qbs *str);
qbs *qbs_lcase(qbs *str);
qbs *func_chr(int32 value);
qbs *func_varptr_helper(uint8 type,uint16 offset);
qbs *qbs_left(qbs *str,int32 l);
qbs *qbs_right(qbs *str,int32 l);

#endif
