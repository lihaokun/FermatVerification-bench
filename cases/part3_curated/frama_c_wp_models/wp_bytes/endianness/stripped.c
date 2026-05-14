/* run.config*
   STDOPT:+"-wp-model bytes+raw -wp-timeout 40 -machdep x86_32 -wp-prop little"
   STDOPT:+"-wp-model bytes+raw -wp-timeout 40 -machdep ppc_32 -wp-prop big"
*/

typedef unsigned long long uint64 ;
typedef unsigned           uint32 ;
typedef unsigned short     uint16 ;
typedef unsigned char      uint8 ;


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
