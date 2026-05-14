/* run.config*
   STDOPT:+"-wp-model bytes+raw -wp-timeout 40 -wp-prop=-skip"
*/

typedef unsigned long long uint64 ;
typedef unsigned           uint32 ;
typedef unsigned short     uint16 ;
typedef unsigned char      uint8 ;

typedef long long          int64 ;
typedef int                int32 ;
typedef short int          int16 ;
typedef signed char        int8 ;

void unsigned_(void){
  uint64 u64 ;
  uint32 u32 ;
  uint16 u16 ;
  uint8  u8 ;
  unsigned write ;

  u64 = 0x1122334455667788ULL ;
  u32 = 0x11223344U ;
  u16 = 0x1122 ;
  u8  = 0x11 ;

  write = 0 ;

  
  
  
  

  
  
  
  
}

void signed_pos(void){
  int64 p64 ;
  int32 p32 ;
  int16 p16 ;
  int8  p8 ;
  unsigned write ;

  p64 = 0x0122334455667788LL ;
  p32 = 0x01223344 ;
  p16 = 0x0122 ;
  p8  = 0x01 ;

  write = 0 ;

  
  
  
  

  
  
  
  
}

void signed_neg(void){
  int64 m64 ;
  int32 m32 ;
  int16 m16 ;
  int8  m8 ;
  unsigned write ;

  m64 = -0x0122334455667788LL ;
  m32 = -0x01223344 ;
  m16 = -0x0122 ;
  m8  = -0x01 ;

  write = 0 ;

  
  
  
  

  
  
  
  
}

void cast_unsigned_signed_pos(void){
  uint64 u64 ;
  uint16 u16 ;
  unsigned write ;

  u64 = 0x0122334455667788ULL ;
  u16 = 0x0122 ;

  
  
  
  
}

void cast_unsigned_signed_neg(void){
  uint64 u64 ;
  uint16 u16 ;
  unsigned write ;

  u64 = 0x8122334455667788ULL ;
  u16 = 0x8182 ;

  
  
  
  
}

void cast_from_bytes_to_unsigned(void){
  uint8 array[] = { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88 };

  uint64 u64_0 = * (uint64 *) array ;

  uint32 u32_0 = * (uint32 *) array ;
  uint32 u32_1 = * (((uint32 *) array) + 1) ;

  uint16 u16_0 = * (uint16 *) array ;
  uint16 u16_1 = * (((uint16 *) array) + 1) ;
  uint16 u16_2 = * (((uint16 *) array) + 2) ;
  uint16 u16_3 = * (((uint16 *) array) + 3) ;

  
  
  
  
  
  
  
}

void cast_from_bytes_to_signed_pos(void){
  uint8 array[] = { 0x11, 0x02, 0x33, 0x04, 0x55, 0x06, 0x77, 0x08 };

  int64 i64_0 = * (int64 *) array ;

  int32 i32_0 = * (int32 *) array ;
  int32 i32_1 = * (((int32 *) array) + 1) ;

  int16 i16_0 = * (int16 *) array ;
  int16 i16_1 = * (((int16 *) array) + 1) ;
  int16 i16_2 = * (((int16 *) array) + 2) ;
  int16 i16_3 = * (((int16 *) array) + 3) ;

  
  
  
  
  
  
  
}

void cast_from_bytes_to_signed_neg(void){
  uint8 array[] = { 0x11, 0x82, 0x33, 0x84, 0x55, 0x86, 0x77, 0x88 };

  int64 i64_0 = * (int64 *) array ;

  int32 i32_0 = * (int32 *) array ;
  int32 i32_1 = * (((int32 *) array) + 1) ;

  int16 i16_0 = * (int16 *) array ;
  int16 i16_1 = * (((int16 *) array) + 1) ;
  int16 i16_2 = * (((int16 *) array) + 2) ;
  int16 i16_3 = * (((int16 *) array) + 3) ;

  
  
  
  
  
  
  
}
