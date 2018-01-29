#include "common.h"
#include "rounding.h"

#ifdef QB64_NOT_X86
int64 qbr(long double f){
  int64 i; int temp=0;
  if (f>9223372036854775807) {temp=1;f=f-9223372036854775808u;} //if it's too large for a signed int64, make it an unsigned int64 and return that value if possible.
  if (f<0) i=f-0.5f; else i=f+0.5f;
  if (temp) return i|0x8000000000000000;//+9223372036854775808;
  return i;
}
uint64 qbr_longdouble_to_uint64(long double f){if (f<0) return(f-0.5f); else return(f+0.5f);}
int32 qbr_float_to_long(float f){if (f<0) return(f-0.5f); else return(f+0.5f);}
int32 qbr_double_to_long(double f){if (f<0) return(f-0.5f); else return(f+0.5f);}
#else
//QBASIC compatible rounding via FPU:
#ifdef QB64_MICROSOFT
int64 qbr(long double f){
  int64 i; int temp=0;
  if (f>9223372036854775807) {temp=1;f=f-9223372036854775808u;} //if it's too large for a signed int64, make it an unsigned int64 and return that value if possible.
  __asm{
      fld   f
      fistp i
      }
  if (temp) return i|0x8000000000000000;//+9223372036854775808;
  return i;
}
uint64 qbr_longdouble_to_uint64(long double f){
  uint64 i;
  __asm{
    fld   f
      fistp i
      }
  return i;
}
int32 qbr_float_to_long(float f){
  int32 i;
  __asm{
    fld   f
      fistp i
      }
  return i;
}
int32 qbr_double_to_long(double f){
  int32 i;
  __asm{
    fld   f
      fistp i
      }
  return i;
}
#else
//FLDS=load single
//FLDL=load double
//FLDT=load long double
int64 qbr(long double f){
  int64 i; int temp=0;
  if (f>9223372036854775807) {temp=1;f=f-9223372036854775808u;} //if it's too large for a signed int64, make it an unsigned int64 and return that value if possible.
  __asm__ (
           "fldt %1;"
           "fistpll %0;"              
           :"=m" (i)
           :"m" (f)
           );
  if (temp) return i|0x8000000000000000;// if it's an unsigned int64, manually set the bit flag
  return i;
}
uint64 qbr_longdouble_to_uint64(long double f){
  uint64 i;
  __asm__ (
           "fldt %1;"
           "fistpll %0;"              
           :"=m" (i)
           :"m" (f)
           );
  return i;
}
int32 qbr_float_to_long(float f){
  int32 i;
  __asm__ (
           "flds %1;"
           "fistpl %0;"              
           :"=m" (i)
           :"m" (f)
           );
  return i;
}
int32 qbr_double_to_long(double f){
  int32 i;
  __asm__ (
           "fldl %1;"
           "fistpl %0;"              
           :"=m" (i)
           :"m" (f)
           );
  return i;
}
#endif
#endif //x86 support

