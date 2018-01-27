#ifndef INC_GFS_H
#define INC_GFS_H

#include "common.h"

/* Generic File System (GFS)
   GFS allows OS specific access whilst still maintaining 'pure' C-based routines for
   multiplatform compatibility. 'Pure' C-based routines may not allow certain functionality,
   such as partial file locking.
   GFS handles/indexes are independent of QB64 handles/indexes to allow for internal files
   to be open but not intefere with the QB64 file handle numbers.

   GFS error codes:
   -1 non-specific fail
   -2 invalid handle
   -3 bad/incorrect file mode
   -4 illegal function call (input is out of range)
   -5 file not found (win:2)
   -6 path not found (win:3)
   -7 access/permission denied (win:5,19)
   -8 device unavailable/drive invalid (win:15,21)
   -9 path/file access error
   -10 read past eof
   -11 bad file name
*/

#ifdef QB64_WINDOWS
#define GFS_WINDOWS
#endif
#ifndef GFS_WINDOWS
#define GFS_C
#endif

struct gfs_file_struct{//info applicable to all files
  int64 id;//a unique ID given to all files (currently only referenced by the FIELD statement to remove old field conditions)
  uint8 open;
  uint8 read;
  uint8 write;
  uint8 lock_read;
  uint8 lock_write;
  int64 pos;//-1=unknown
  uint8 eof_reached;//read last character of file (set/reset by gfs_read only)
  uint8 eof_passed;//attempted to read past eof (set/reset by gfs_read only)
  int32 fileno;//link to fileno index
  uint8 type;//qb access method (1=RANDOM,2=BINARY,3=INPUT,4=OUTPUT)
  int64 record_length;//used by RANDOM
  uint8 *field_buffer;
  qbs **field_strings;//list of qbs pointers linked to this file
  int32 field_strings_n;//number of linked strings
  int64 column;//used by OUTPUT/APPEND to tab correctly (base 0)
  //GFS_C data follows: (unused by custom GFS interfaces)
  fstream *file_handle;
  ofstream *file_handle_o;
  //COM port data follows (*=default)
  uint8 com_port;//0=not a com port
  int32 com_baud_rate;//(bits per second)75,110,150,300*,600,1200,1800,2400,9600,?
  int8 com_parity;//[0]N,[1]E*,[2]O,[3]S,[4]M,[5]PE(none,even*,odd,space,mark,error-checking)
  int8 com_data_bits_per_byte;//5,6,7*,8
  int8 com_stop_bits;//[10]1,[15]1.5,[20]2
  //The default value is 1 for baud rates greater than 110. For
  //baud rates less than or equal to 110, the default value is
  //1.5 when data is 5; otherwise, the value is 2.
  int8 com_bin_asc;//[0]=BIN*,[1]=ASC
  int8 com_asc_lf;//[0]omit*,[1]LF(only valid with ASC)
  //note: rb_x and tb_x are ignored by QB64 (receive and transmit buffer sizes)
  int8 com_rs;//[0]detect*,[1]dont-detect
  int32 com_cd_x;//0*-65535
  int32 com_cs_x;//1000*,0-65535
  int32 com_ds_x;//1000*,0-65535
  int32 com_op_x;
  //                 OP not used:          x omitted:     x specified:
  //                 10 times the CD or    10000 ms       0 - 65,535 milliseconds
  //                 DS timeout value,
  //                 whichever is greater

  //SCRN: support follows
  uint8 scrn; //0 = not a file opened as "SCRN:"
};

#ifdef GFS_WINDOWS
struct gfs_file_win_struct{//info applicable to WINDOWS OS files
  HANDLE file_handle;
};
extern gfs_file_win_struct *gfs_file_win;
#endif

extern int64 gfs_nextid;
extern gfs_file_struct *gfs_file;

extern int32 gfs_n;
extern int32 gfs_freed_n;
extern int32 *gfs_freed;
extern int32 gfs_freed_size;

extern int32 *gfs_fileno;
extern int32 gfs_fileno_n;

extern int64 gfs_read_bytes_value;

int32 gfs_new();
int32 gfs_validhandle(int32 i);
int32 gfs_fileno_valid(int32 f);
int32 gfs_fileno_freefile();
void gfs_fileno_use(int32 f, int32 i);
void gfs_fileno_free(int32 f);
int32 gfs_free(int32 i);
int32 gfs_close(int32 i);
int64 gfs_lof(int32 i);
int32 gfs_open_com_syntax(qbs *fstr,gfs_file_struct *f);
int32 gfs_open(qbs *filename,int32 access,int32 restrictions, int32 how);
int32 gfs_setpos(int32 i, int64 position);
int64 gfs_getpos(int32 i);
int32 gfs_write(int32 i,int64 position,uint8 *data,int64 size);
int64 gfs_read_bytes();
int32 gfs_read(int32 i,int64 position,uint8 *data,int64 size);
int32 gfs_eof_reached(int32 i);
int32 gfs_eof_passed(int32 i);
int32 gfs_lock(int32 i,int64 offset_start,int64 offset_end);
int32 gfs_unlock(int32 i,int64 offset_start,int64 offset_end);
#endif
